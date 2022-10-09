/**********************************************************************************
*\file         paperback_pool_inline.h
*\brief        paperback_pool_inline.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace paperback::vm
{
	//-----------------------------------
	//            Default
	//-----------------------------------
	instance::~instance( void ) noexcept
	{
		Clear();

		for ( auto cPool : m_MemoryPool )
		{
			if ( cPool )
				VirtualFree( cPool, 0, MEM_RELEASE );
		}
	}

	void instance::Init( std::span<const component::info* const> Types, const u32 NumComponents, paperback::coordinator::instance* Coordinator ) noexcept
	{
		m_ComponentInfo		 = Types;
		m_NumberOfComponents = NumComponents;
		m_pCoordinator	     = Coordinator;

		// Reserve memory required for MaxEntites
		for ( std::size_t i = 0; i < m_NumberOfComponents; i++ )
		{
			auto nPages = GetPageIndex( *m_ComponentInfo[i], settings::max_entities_v ) + 1;
			m_MemoryPool[i] = reinterpret_cast<std::byte*>( VirtualAlloc(nullptr, nPages * paperback::settings::virtual_page_size_v, MEM_RESERVE, PAGE_NOACCESS) );

			PPB_ASSERT_MSG( !m_MemoryPool[i], "Pool Init - Memory is not reserved" );
		}
	}


	//-----------------------------------
	//         Create / Delete
	//-----------------------------------
	u32 instance::Append( void ) noexcept
	{
		if ( m_CurrentEntityCount + 1 >= settings::max_entities_v )
        {
            ERROR_PRINT( "Pool Append: Maximum entities reached" );
            ERROR_LOG( "Pool Append: Maximum entities reached" );

			return settings::invalid_index_v;
        }

		// For each valid component
		for (size_t i = 0, end = m_NumberOfComponents; i < end; i++)
		{
			auto CInfo	   = *m_ComponentInfo[i];
			auto iCurPage  =  GetPageIndex( CInfo, m_CurrentEntityCount );
			auto iNextpage =  GetPageIndex( CInfo, m_CurrentEntityCount + 1 );

			// Commit new memory in m_ComponentPool's page if the page is full
			if ( iCurPage != iNextpage )
			{
				auto pEndOfCurrentPool = m_MemoryPool[i] + iNextpage * paperback::settings::virtual_page_size_v;
				auto pNewPool = VirtualAlloc( pEndOfCurrentPool, paperback::settings::virtual_page_size_v, MEM_COMMIT, PAGE_READWRITE );

				PPB_ASSERT_MSG( pNewPool != pEndOfCurrentPool, "Pool Append - Memory is not contiguous" );
			}

			// Invoke constructor for Component (If Required)
			if ( m_ComponentInfo[i]->m_Constructor )
			{
				m_ComponentInfo[i]->m_Constructor( m_MemoryPool[i] + m_CurrentEntityCount * m_ComponentInfo[i]->m_Size );
			}
		}

		return m_CurrentEntityCount++;
	}

	PPB_INLINE
	void instance::DestroyEntity( paperback::component::entity& Entity ) noexcept
	{
		PPB_ASSERT_MSG( Entity.IsZombie(), "DestroyEntity: Attemping to double delete an entity" );

        auto& EntityInfo = m_pCoordinator->GetEntityInfo( Entity );
        auto& PoolEntity = GetComponent<component::entity>( EntityInfo.m_PoolDetails.m_PoolIndex );

        PPB_ASSERT_MSG( &Entity != &PoolEntity, "DestroyEntity: Entity addresses are different" );

        Entity.m_Validation.m_bZombie
            = EntityInfo.m_Validation.m_bZombie
            = true;

        EntityInfo.m_Validation.m_Next = m_DeleteHead;
        m_DeleteHead = PoolEntity.m_GlobalIndex;
	}

	u32 instance::Delete( const u32 PoolIndex ) noexcept
	{
		PPB_ASSERT_MSG( PoolIndex >= m_CurrentEntityCount || PoolIndex < 0,
						"Pool Delete - Invalid pool index" );

		// Return back to the current index
		--m_CurrentEntityCount;
		const auto EntityGlobalIndex = GetComponent<component::entity>(PoolIndex).m_GlobalIndex;

		for ( size_t i = 0; i < m_NumberOfComponents; ++i )
		{
			const auto& pInfo = *m_ComponentInfo[i];
			auto		pData =  m_MemoryPool[i];

			// Unlink Parent & Child relationship on deletion of entity
			//UnlinkParentAndChildOnDelete( pInfo, PoolIndex, EntityGlobalIndex );

			if ( pInfo.m_Guid == component::info_v<parent>.m_Guid )
				m_pCoordinator->BroadcastEvent<OnEvent_ParentDeleted>( GetComponent<parent>( PoolIndex ) );
			else if ( pInfo.m_Guid == component::info_v<child>.m_Guid )
				m_pCoordinator->BroadcastEvent<OnEvent_ChildDeleted>( GetComponent<child>( PoolIndex ), EntityGlobalIndex );
			else if (pInfo.m_Guid == component::info_v<reference_prefab>.m_Guid)
				m_pCoordinator->BroadcastEvent< OnEvent_ReferencePrefabDeleted >(GetComponent<reference_prefab>(PoolIndex), EntityGlobalIndex);
			
			// Abandon Prefab Instances when Deleting Prefab
			//AbandonPrefabInstancesOnPrefabDelete( pInfo, EntityGlobalIndex );

			// Deleting last Entity
			if ( PoolIndex == m_CurrentEntityCount )
			{
				if ( pInfo.m_Destructor )
					pInfo.m_Destructor( &pData[m_CurrentEntityCount * pInfo.m_Size] );
			}
			// Deleting any other Entity
			else
			{
				if ( pInfo.m_Move )
				{
					pInfo.m_Move( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size] );
					if ( pInfo.m_Destructor ) pInfo.m_Destructor( &pData[m_CurrentEntityCount * pInfo.m_Size] );
				}
				else
				{
					if ( pInfo.m_Destructor )
						pInfo.m_Destructor(&pData[PoolIndex * pInfo.m_Size]);
					memcpy( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size], pInfo.m_Size );
				}
			}

			// Free the page if empty
			const auto LastPage = GetPageIndex( pInfo, m_CurrentEntityCount + 1 );
			if ( LastPage != GetPageIndex(pInfo, m_CurrentEntityCount) )
			{
				auto pRaw = &pData[ paperback::settings::virtual_page_size_v * LastPage ];
				auto b = VirtualFree( pRaw, paperback::settings::virtual_page_size_v, MEM_DECOMMIT );

				PPB_ASSERT_MSG( !b, "Pool Delete - Virtual free failed" );
			}
		}

		return ( m_CurrentEntityCount == 0 || PoolIndex >= m_CurrentEntityCount )
				 ? UINT32_MAX
				 : GetComponent<component::entity>( PoolIndex ).m_GlobalIndex;
	}

	bool instance::RemoveTransferredEntity( const u32 PoolIndex ) noexcept
	{
		PPB_ASSERT_MSG( PoolIndex < 0, "Pool RemoveTransferredEntity - Invalid pool index");

		// Backtrack to last valid entity
		if ( PoolIndex >= m_CurrentEntityCount )
			return false;

		// Update counter
		--m_CurrentEntityCount;

		for ( size_t i = 0; i < m_NumberOfComponents; ++i )
		{
			const auto& pInfo = *m_ComponentInfo[i];
			auto		pData =  m_MemoryPool[i];

			// If moving last entity - Just destroy it
			if ( PoolIndex == m_CurrentEntityCount )
			{
				if ( pInfo.m_Destructor ) pInfo.m_Destructor( &pData[m_CurrentEntityCount * pInfo.m_Size] );
			}
			// Moving any other entity - Copy memory and shift forward
			else
			{
				if ( pInfo.m_Move )
				{
					pInfo.m_Move( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size] );
				}
				else
				{
					memcpy( &pData[PoolIndex * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size], pInfo.m_Size );
				}
				if ( pInfo.m_Destructor ) pInfo.m_Destructor( &pData[m_CurrentEntityCount * pInfo.m_Size] );
			}

			// Free the page if empty
			const auto LastPage = GetPageIndex( pInfo, m_CurrentEntityCount + 1 );
			if ( LastPage != GetPageIndex(pInfo, m_CurrentEntityCount) )
			{
				auto pRaw = &pData[ paperback::settings::virtual_page_size_v * LastPage ];
				auto b = VirtualFree( pRaw, paperback::settings::virtual_page_size_v, MEM_DECOMMIT );

				PPB_ASSERT_MSG( !b, "Pool RemoveTransferredEntity - Virtual free failed" );
			}
		}
		
		// Update the "Last Entity" that was moved to fill the gap in "PoolIndex"
		if ( m_CurrentEntityCount != 0 )
		{
			auto& MovedEntity = GetComponent<paperback::component::entity>( PoolIndex );
			auto& MovedEntityInfo = m_pCoordinator->GetEntityInfo( MovedEntity );
			MovedEntityInfo.m_PoolDetails.m_PoolIndex = PoolIndex;
		}


		return true;
	}

	void instance::Clear() noexcept
	{
		while ( m_CurrentEntityCount )
		{
			Delete( m_CurrentEntityCount-1 );
		}
	}

	PPB_INLINE
	void instance::UpdateStructuralChanges( void ) noexcept
	{
		auto ResetInfo  = [&]( paperback::entity::info& Info ) { Info.m_Validation.m_UID = 0; Info.m_pArchetype = nullptr; };
		auto ResetMInfo = [&]( paperback::entity::info& Info ) { Info.m_Validation.m_UID = 0; };

		while ( m_DeleteHead != settings::invalid_delete_index_v )
		{
			auto& Info = m_pCoordinator->GetEntityInfo( m_DeleteHead );
            m_pCoordinator->RemoveEntity( Delete( Info.m_PoolDetails.m_PoolIndex ), m_DeleteHead );
			m_DeleteHead = Info.m_Validation.m_Next;
			ResetInfo( Info );
		}

        while ( m_MoveHead != settings::invalid_delete_index_v )
        {
			auto& Info = m_pCoordinator->GetEntityInfo( m_MoveHead );
            RemoveTransferredEntity( Info.m_PoolDetails.m_PoolIndex );
			m_MoveHead = Info.m_Validation.m_Next;
			ResetMInfo( Info );
        }
	}


	//-----------------------------------
	//             Transfer
	//-----------------------------------

	u32 instance::TransferExistingComponents( const PoolDetails& Details, vm::instance& FromPool ) noexcept
	{
		const u32 NewPoolIndex = Append();

        u32 iPoolFrom = 0;
        u32 iPoolTo   = 0;

		auto& From_MemoryPool = FromPool.GetMemoryPool();

        while( true )
        {
			// Component exists in both - Copy existing component
			if ( FromPool.m_ComponentInfo[ iPoolFrom ]->m_Guid == m_ComponentInfo[ iPoolTo ]->m_Guid )
            {
                auto& Info = *( FromPool.m_ComponentInfo[ iPoolFrom ] );

                if( Info.m_Move )
                {
                    Info.m_Move( &m_MemoryPool[ iPoolTo ][ Info.m_Size * NewPoolIndex ]					    // Destination
							   , &From_MemoryPool[ iPoolFrom ][ Info.m_Size * Details.m_PoolIndex ] );		// Source
                }
                else
                {
                    std::memcpy( &m_MemoryPool[ iPoolTo ][ Info.m_Size * NewPoolIndex ]						// Destination
                               , &From_MemoryPool[ iPoolFrom ][ Info.m_Size * Details.m_PoolIndex ]			// Source
                               , Info.m_Size );																// Number of bytes to copy
                }

				// If either pool's components is maxed out
                if ( ++iPoolFrom >= FromPool.m_ComponentInfo.size() || ++iPoolTo >= m_ComponentInfo.size() ) break;
            }
			// Component has been removed in new archetype - Destroy old component
            else if ( FromPool.m_ComponentInfo[ iPoolFrom ]->m_UID < m_ComponentInfo[ iPoolTo ]->m_UID )
            {
                if ( ++iPoolFrom >= FromPool.m_ComponentInfo.size() ) break;
            }
			// Component does not exist in old pool - Continue
            else
            {
                if ( ++iPoolTo >= m_ComponentInfo.size() ) break;
            }
        }

		FromPool.MarkEntityAsMoved( Details.m_PoolIndex );

        return NewPoolIndex;
	}
	

	//-----------------------------------
	//              Clone
	//-----------------------------------

	// Clone Components within the Same Pool
	void instance::CloneComponents( const u32 ToIndex, const u32 FromIndex ) noexcept
	{
		for ( u32 i = 0; i < m_NumberOfComponents; ++i )
		{
			// If component to be cloned is the parent component
			if ( m_ComponentInfo[ i ]->m_Guid == component::info_v<parent>.m_Guid )
			{
				// Grab parent component from entity that is to be cloned
				auto& ToParent     = GetComponent<parent>( ToIndex );
				auto& ToEntity     = GetComponent<paperback::component::entity>( ToIndex );
				auto& FromParent   = GetComponent<parent>( FromIndex );
				auto& ChildrenList = FromParent.m_ChildrenGlobalIndexes;

				// Grab global index of each child
				for ( const auto& ChildGID : ChildrenList )
				{
					// Clone a copy of the child
					auto& CInfo         = m_pCoordinator->GetEntityInfo( ChildGID );
					auto ClonedChildGID = CInfo.m_pArchetype->CloneEntity( CInfo.m_pArchetype->GetComponent<paperback::component::entity>( CInfo.m_PoolDetails ) );

					// Grab info of cloned child
					auto& ClonedInfo    = m_pCoordinator->GetEntityInfo( ClonedChildGID );
					auto& ClonedChild   = ClonedInfo.m_pArchetype->GetComponent<child>( ClonedInfo.m_PoolDetails );

					// Assign parent/child relationship
					ToParent.AddChild( ClonedChildGID );
					ClonedChild.AddParent( ToEntity.m_GlobalIndex );
				}
			}
			// Else if component to be cloned is the child / entity component
			else if ( m_ComponentInfo[ i ]->m_Guid == component::info_v<child>.m_Guid ||
				      m_ComponentInfo[ i ]->m_Guid == component::info_v<paperback::component::entity>.m_Guid )
			{
				continue;
			}
			// Else, any other component to be cloned
			else
			{
				auto CSize = m_ComponentInfo[ i ]->m_Size;

				if ( m_ComponentInfo[ i ]->m_Copy )
				{
						m_ComponentInfo[i]->m_Copy( &m_MemoryPool[ i ][ ToIndex * CSize ]		// Destination
												  , &m_MemoryPool[ i ][ FromIndex * CSize ] );	// Source
				}
				else
				{
						std::memcpy( &m_MemoryPool[ i ][ CSize * ToIndex ]						// Destination
								   , &m_MemoryPool[ i ][ CSize * FromIndex ]					// Source
								   , CSize );													// Number of bytes to copy
				}
			}
		}
	}

	// Clone Components within a different pool
	const u32 instance::CloneComponents( const u32 ToIndex, const u32 FromIndex, vm::instance& FromPool ) noexcept
	{
        u32 iPoolFrom = 0;
        u32 iPoolTo   = 0;

		auto& From_MemoryPool = FromPool.GetMemoryPool();

        while( true )
        {
			// Component exists in both - Copy existing component
			if ( FromPool.m_ComponentInfo[ iPoolFrom ]->m_Guid == m_ComponentInfo[ iPoolTo ]->m_Guid )
            {
				// If component to be cloned is the parent component
				if ( m_ComponentInfo[ iPoolTo ]->m_Guid == component::info_v<parent>.m_Guid )
				{
					// Grab parent component from entity that is to be cloned
					auto& ToParent     = GetComponent<parent>( ToIndex );
					auto& ToEntity     = GetComponent<paperback::component::entity>( ToIndex );
					auto& FromParent   = FromPool.GetComponent<parent>( FromIndex );
					auto& ChildrenList = FromParent.m_ChildrenGlobalIndexes;

					// Grab global index of each child
					for ( const auto& ChildGID : ChildrenList )
					{
						// Clone a copy of the child
						auto& CInfo         = m_pCoordinator->GetEntityInfo( ChildGID );
						auto ClonedChildGID = CInfo.m_pArchetype->ClonePrefab( CInfo.m_PoolDetails.m_PoolIndex );
						//auto ClonedChildGID = CInfo.m_pArchetype->CloneEntity( CInfo.m_pArchetype->GetComponent<paperback::component::entity>( CInfo.m_PoolDetails ) );

						// Grab info of cloned child
						auto& ClonedInfo    = m_pCoordinator->GetEntityInfo( ClonedChildGID );
						auto& ClonedChild   = ClonedInfo.m_pArchetype->GetComponent<child>( ClonedInfo.m_PoolDetails );

						// Assign parent/child relationship
						ToParent.AddChild( ClonedChildGID );
						ClonedChild.AddParent( ToEntity.m_GlobalIndex );
					}

					++iPoolFrom;
					++iPoolTo;
				}
				// Else if component to be cloned is the child / entity component
				else if ( m_ComponentInfo[ iPoolTo ]->m_Guid == component::info_v<child>.m_Guid ||
						  m_ComponentInfo[ iPoolTo ]->m_Guid == component::info_v<paperback::component::entity>.m_Guid )
				{
					++iPoolFrom;
					++iPoolTo;
					continue;
				}
				// Else, any other component to be cloned
				else
				{
					auto& Info = *( FromPool.m_ComponentInfo[ iPoolFrom ] );

					if( Info.m_Copy )
					{
						Info.m_Copy( &m_MemoryPool[ iPoolTo ][ Info.m_Size * ToIndex ]					    // Destination
								   , &From_MemoryPool[ iPoolFrom ][ Info.m_Size * FromIndex ] );		    // Source
					}
					else
					{
						std::memcpy( &m_MemoryPool[ iPoolTo ][ Info.m_Size * ToIndex ]						// Destination
								   , &From_MemoryPool[ iPoolFrom ][ Info.m_Size * FromIndex ]			    // Source
								   , Info.m_Size );													        // Number of bytes to copy
					}

					// If either pool's components is maxed out
					if ( ++iPoolFrom >= FromPool.m_ComponentInfo.size() || ++iPoolTo >= m_ComponentInfo.size() ) break;
				} 
            }
			// Component does not exist in new entity - Continue
            else if ( FromPool.m_ComponentInfo[ iPoolFrom ]->m_UID < m_ComponentInfo[ iPoolTo ]->m_UID )
            {
                if ( ++iPoolFrom >= FromPool.m_ComponentInfo.size() ) break;
            }
			// Component does not exist in old pool - Continue
            else
            {
                if ( ++iPoolTo >= m_ComponentInfo.size() ) break;
            }
        }

        return ToIndex;
	}


	//-----------------------------------
	//              Save
	//-----------------------------------

	void instance::SerializePoolComponentsAtEntityIndex( const u32 Index, paperback::JsonFile& Jfile ) noexcept
	{
		for ( u32 i = 0, max = m_NumberOfComponents; i < max; ++i )
		{
			rttr::instance Component = GetComponentInstance( m_ComponentInfo[i]->m_Guid, Index );
			Jfile.WriteKey( Component.get_type().get_name().to_string() ).StartObject();
			Jfile.Write( Component );
			Jfile.EndObject();
		}
	}


	//-----------------------------------
	//            Getters
	//-----------------------------------

	template < typename T_COMPONENT >
	T_COMPONENT* instance::FindComponent( const u32 PoolIndex ) const noexcept
	{
		//auto ComponentIndex = GetComponentIndex( component::info_v<T_COMPONENT>.m_UID );
		auto ComponentIndex = GetComponentIndex( component::info_v<T_COMPONENT>.m_Guid );

		if ( ComponentIndex == -1 ) return nullptr;

		return reinterpret_cast< std::decay_t<T_COMPONENT>* >
		(
			&m_MemoryPool[ ComponentIndex ][ PoolIndex * m_ComponentInfo[ComponentIndex]->m_Size] 
		);
	}

	std::byte* instance::FindComponent( const u32 PoolIndex, const component::type::guid ComponentGuid) const noexcept
	{
		//auto ComponentIndex = GetComponentIndex( component::info_v<T_COMPONENT>.m_UID );
		auto ComponentIndex = GetComponentIndex( ComponentGuid );

		if ( ComponentIndex == -1 ) return nullptr;

		return &m_MemoryPool[ ComponentIndex ][ PoolIndex * m_ComponentInfo[ComponentIndex]->m_Size];
	}

	template < typename T_COMPONENT >
	T_COMPONENT& instance::GetComponent( const u32 PoolIndex ) const noexcept
	{
		//auto ComponentIndex = GetComponentIndex( component::info_v<T_COMPONENT>.m_UID );
		auto ComponentIndex = GetComponentIndex( component::info_v<T_COMPONENT>.m_Guid );

		return *reinterpret_cast< std::decay_t<T_COMPONENT>* >
		(
			&m_MemoryPool[ ComponentIndex ][ PoolIndex * m_ComponentInfo[ComponentIndex]->m_Size] 
		);
	}

	int instance::GetComponentIndex( const u32 UIDComponent ) const noexcept
	{
		// Find index of component within m_ComponentPool
		for ( size_t i = 0, end = m_NumberOfComponents; i < end; ++i )
			if ( m_ComponentInfo[i]->m_UID == UIDComponent ) { return static_cast<int>(i); }

		return -1;
	}

	int instance::GetComponentIndex(const component::type::guid Guid) const noexcept
	{
		// Find index of component within m_ComponentPool
		for ( size_t i = 0, end = m_NumberOfComponents; i < end; ++i )
			if ( m_ComponentInfo[i]->m_Guid == Guid ) { return static_cast<int>(i); }

		return -1;
	}

	int instance::GetComponentIndexFromGUID( const component::type::guid Guid ) const noexcept
	{
		for ( u32 i = 0, max = m_NumberOfComponents; i < max; ++i )
		{
			if ( m_ComponentInfo[i]->m_Guid.m_Value == Guid.m_Value )
				return i;
		}
		return -1;
	}

	int instance::GetComponentIndexFromGUIDInSequence( const component::type::guid Guid, const int Sequence ) const noexcept
	{
		for ( u32 i = Sequence, max = m_NumberOfComponents; i < max; ++i )
		{
			if ( m_ComponentInfo[i]->m_Guid.m_Value == Guid.m_Value )
				return i;
		}
		return -1;
	}

	std::vector< std::pair < rttr::instance, paperback::component::type::guid> > instance::GetComponents( const u32 Index ) noexcept
	{
		std::vector < std::pair < rttr::instance, paperback::component::type::guid > > ComponentList = {};

		for (size_t i = 0, max = m_ComponentInfo.size(); i < max; ++i)
		{
			auto ComponentInst = GetComponentInstance(m_ComponentInfo[i]->m_Guid, Index);
			auto& ComponentGuid = m_ComponentInfo[i]->m_Guid;
			ComponentList.push_back(std::make_pair(ComponentInst, ComponentGuid));
		}
		return ComponentList;
	}

	rttr::instance instance::GetComponentInstance( const component::type::guid Comp_Guid, const u32 Index ) noexcept
	{

		if ( Comp_Guid.m_Value == component::info_v< component::entity >.m_Guid.m_Value )
			return rttr::instance( GetComponent< component::entity >( Index ));
		else if (Comp_Guid.m_Value == component::info_v< text >.m_Guid.m_Value)
			return rttr::instance(GetComponent< text >(Index));
		else if ( Comp_Guid.m_Value == component::info_v< transform >.m_Guid.m_Value )
			return rttr::instance( GetComponent< transform >( Index ));
		else if (Comp_Guid.m_Value == component::info_v< spawner >.m_Guid.m_Value)
			return rttr::instance(GetComponent< spawner >(Index));
		else if (Comp_Guid.m_Value == component::info_v< card >.m_Guid.m_Value)
			return rttr::instance(GetComponent< card >(Index));
		else if (Comp_Guid.m_Value == component::info_v< deck >.m_Guid.m_Value)
			return rttr::instance(GetComponent< deck >(Index));
		else if (Comp_Guid.m_Value == component::info_v< scale >.m_Guid.m_Value)
			return rttr::instance(GetComponent< scale >(Index));
		else if (Comp_Guid.m_Value == component::info_v< rotation >.m_Guid.m_Value)
			return rttr::instance(GetComponent< rotation >(Index));
		else if (Comp_Guid.m_Value == component::info_v< rigidbody >.m_Guid.m_Value)
			return rttr::instance(GetComponent< rigidbody >(Index));
		else if (Comp_Guid.m_Value == component::info_v< timer >.m_Guid.m_Value)
			return rttr::instance(GetComponent< timer >(Index));
		else if (Comp_Guid.m_Value == component::info_v< mesh >.m_Guid.m_Value)
			return rttr::instance(GetComponent< mesh >(Index));
		else if (Comp_Guid.m_Value == component::info_v< sound >.m_Guid.m_Value)
			return rttr::instance(GetComponent< sound >(Index));
		else if (Comp_Guid.m_Value == component::info_v< entityscript >.m_Guid.m_Value)
			return rttr::instance(GetComponent< entityscript >(Index));
		else if (Comp_Guid.m_Value == component::info_v< collidable >.m_Guid.m_Value)
			return rttr::instance(GetComponent< collidable >(Index));
		else if (Comp_Guid.m_Value == component::info_v< rigidforce >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< rigidforce >(Index));
		else if (Comp_Guid.m_Value == component::info_v< boundingbox >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< boundingbox >(Index));
		else if (Comp_Guid.m_Value == component::info_v< sphere >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< sphere >(Index));
		else if (Comp_Guid.m_Value == component::info_v< icon >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< icon >(Index));
		else if (Comp_Guid.m_Value == component::info_v< animator >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< animator >(Index));
		else if (Comp_Guid.m_Value == component::info_v< parent >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< parent >(Index));
		else if (Comp_Guid.m_Value == component::info_v< child >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< child >(Index));
		else if (Comp_Guid.m_Value == component::info_v< offset >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< offset >(Index));
		else if (Comp_Guid.m_Value == component::info_v< socketed >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< socketed >(Index));
		else if (Comp_Guid.m_Value == component::info_v< prefab >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< prefab >(Index));
		else if (Comp_Guid.m_Value == component::info_v< reference_prefab >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< reference_prefab >(Index));
		else if (Comp_Guid.m_Value == component::info_v< damage >.m_Guid.m_Value)
			return  rttr::instance(GetComponent< damage >(Index));
		else if (Comp_Guid.m_Value == component::info_v< listener >.m_Guid.m_Value)
			return rttr::instance(GetComponent< listener >(Index));


		else if (Comp_Guid.m_Value == component::info_v< name >.m_Guid.m_Value)
			return rttr::instance(GetComponent< name >(Index));
		else if (Comp_Guid.m_Value == component::info_v< cost >.m_Guid.m_Value)
			return rttr::instance(GetComponent< cost >(Index));
		else if (Comp_Guid.m_Value == component::info_v< counter >.m_Guid.m_Value)
			return rttr::instance(GetComponent< counter >(Index));
		else if (Comp_Guid.m_Value == component::info_v< currency >.m_Guid.m_Value)
			return rttr::instance(GetComponent< currency >(Index));
		else if (Comp_Guid.m_Value == component::info_v< enemy >.m_Guid.m_Value)
			return rttr::instance(GetComponent< enemy >(Index));
		else if (Comp_Guid.m_Value == component::info_v< enemy_spawner >.m_Guid.m_Value)
			return rttr::instance(GetComponent< enemy_spawner >(Index));
		else if (Comp_Guid.m_Value == component::info_v< friendly >.m_Guid.m_Value)
			return rttr::instance(GetComponent< friendly >(Index));
		else if (Comp_Guid.m_Value == component::info_v< friendly_spawner >.m_Guid.m_Value)
			return rttr::instance(GetComponent< friendly_spawner >(Index));
		else if (Comp_Guid.m_Value == component::info_v< health >.m_Guid.m_Value)
			return rttr::instance(GetComponent< health >(Index));
		else if (Comp_Guid.m_Value == component::info_v< selected >.m_Guid.m_Value)
			return rttr::instance(GetComponent< selected >(Index));
		else if (Comp_Guid.m_Value == component::info_v< waypoint >.m_Guid.m_Value)
			return rttr::instance(GetComponent< waypoint >(Index));
		else if (Comp_Guid.m_Value == component::info_v< player >.m_Guid.m_Value)
			return rttr::instance(GetComponent< player >(Index));
		else if (Comp_Guid.m_Value == component::info_v< waypointv1 >.m_Guid.m_Value)
			return rttr::instance(GetComponent< waypointv1 >(Index));
		else if (Comp_Guid.m_Value == component::info_v< unitstate >.m_Guid.m_Value)
			return rttr::instance(GetComponent< unitstate >(Index));
		else if (Comp_Guid.m_Value == component::info_v< waypoint_tag >.m_Guid.m_Value)
			return rttr::instance(GetComponent< waypoint_tag >(Index));
		else if (Comp_Guid.m_Value == component::info_v< path >.m_Guid.m_Value)
			return rttr::instance(GetComponent< path >(Index));
		else if (Comp_Guid.m_Value == component::info_v< path_follower >.m_Guid.m_Value)
			return rttr::instance(GetComponent< path_follower >(Index));

		else if (Comp_Guid.m_Value == component::info_v< healthbar >.m_Guid.m_Value)
			return rttr::instance(GetComponent< healthbar >(Index));
		else if (Comp_Guid.m_Value == component::info_v< base >.m_Guid.m_Value)
			return rttr::instance(GetComponent< base >(Index));
		else if (Comp_Guid.m_Value == component::info_v< unit >.m_Guid.m_Value)
			return rttr::instance(GetComponent< unit >(Index));
		else if (Comp_Guid.m_Value == component::info_v< button >.m_Guid.m_Value)
			return rttr::instance(GetComponent< button >(Index));
		else if (Comp_Guid.m_Value == component::info_v< mass >.m_Guid.m_Value)
			return rttr::instance(GetComponent< mass >(Index));

		else if (Comp_Guid.m_Value == component::info_v< Frustum >.m_Guid.m_Value)
			return rttr::instance(GetComponent< Frustum >(Index));
		else if (Comp_Guid.m_Value == component::info_v< LineSegment >.m_Guid.m_Value)
			return rttr::instance(GetComponent< LineSegment >(Index));
		else if (Comp_Guid.m_Value == component::info_v< Plane >.m_Guid.m_Value)
			return rttr::instance(GetComponent< Plane >(Index));
		else if (Comp_Guid.m_Value == component::info_v< Ray >.m_Guid.m_Value)
			return rttr::instance(GetComponent< Ray >(Index));
		else if (Comp_Guid.m_Value == component::info_v< Triangle >.m_Guid.m_Value)
			return rttr::instance(GetComponent< Triangle >(Index));
		else if (Comp_Guid.m_Value == component::info_v< capture_point >.m_Guid.m_Value)
			return rttr::instance(GetComponent< capture_point >(Index));
		else if (Comp_Guid.m_Value == component::info_v< spawner_details >.m_Guid.m_Value)
			return rttr::instance(GetComponent< spawner_details >(Index));
		else if (Comp_Guid.m_Value == component::info_v< player_controller >.m_Guid.m_Value)
			return rttr::instance(GetComponent< player_controller >(Index));
		else if (Comp_Guid.m_Value == component::info_v< camera >.m_Guid.m_Value)
			return rttr::instance(GetComponent< camera >(Index));
		else if (Comp_Guid.m_Value == component::info_v< slope >.m_Guid.m_Value)
			return rttr::instance(GetComponent< slope >(Index));
		else if (Comp_Guid.m_Value == component::info_v< player_interaction >.m_Guid.m_Value)
			return rttr::instance(GetComponent< player_interaction >(Index));
		else if (Comp_Guid.m_Value == component::info_v< pushable >.m_Guid.m_Value)
			return rttr::instance(GetComponent< pushable >(Index));
		else if (Comp_Guid.m_Value == component::info_v< freezable >.m_Guid.m_Value)
		return rttr::instance(GetComponent< freezable >(Index));
		else if (Comp_Guid.m_Value == component::info_v< elevator >.m_Guid.m_Value)
			return rttr::instance(GetComponent< elevator >(Index));
		else if (Comp_Guid.m_Value == component::info_v< crosshair >.m_Guid.m_Value)
			return rttr::instance(GetComponent< crosshair >(Index));
		else if (Comp_Guid.m_Value == component::info_v< light >.m_Guid.m_Value)
			return rttr::instance(GetComponent< light >(Index));
		else if (Comp_Guid.m_Value == component::info_v< bulksound >.m_Guid.m_Value)
			return rttr::instance(GetComponent< bulksound >(Index));
		else if (Comp_Guid.m_Value == component::info_v< cinematic >.m_Guid.m_Value)
			return rttr::instance(GetComponent< cinematic >(Index));
		else if (Comp_Guid.m_Value == component::info_v< ability_collectible >.m_Guid.m_Value)
			return rttr::instance(GetComponent< ability_collectible >(Index));
		else if (Comp_Guid.m_Value == component::info_v< dialogue_text >.m_Guid.m_Value)
			return rttr::instance(GetComponent< dialogue_text >(Index));
		else if (Comp_Guid.m_Value == component::info_v< dialogue_collider >.m_Guid.m_Value)
			return rttr::instance(GetComponent< dialogue_collider >(Index));
		else if (Comp_Guid.m_Value == component::info_v< particle >.m_Guid.m_Value)
			return rttr::instance(GetComponent< particle >(Index));
		else if (Comp_Guid.m_Value == component::info_v< particle_emitter >.m_Guid.m_Value)
			return rttr::instance(GetComponent< particle_emitter >(Index));
		else if (Comp_Guid.m_Value == component::info_v< pure_collider_tag >.m_Guid.m_Value)
			return rttr::instance(GetComponent< pure_collider_tag >(Index));
		else
			return rttr::instance();
	}

	const u32 instance::GetCurrentEntityCount( void ) const noexcept
	{
		return m_CurrentEntityCount;
	}

	const u32 instance::GetComponentCount( void ) const noexcept
	{
		return static_cast<u32>( m_ComponentInfo.size() );
	}



	paperback::vm::instance::MemoryPool& instance::GetMemoryPool( void ) noexcept
	{
		return m_MemoryPool;
	}

	u32 instance::GetPageIndex( const size_t LocalComponentIndex, const u32 Count ) const noexcept
	{
		return ( m_ComponentInfo[LocalComponentIndex]->m_Size * Count ) / settings::virtual_page_size_v;
	}

	u32 instance::GetPageIndex( const component::info& Info, const u32 Count ) const noexcept
	{
		return ( ( Info.m_Size * Count ) - 1 ) / settings::virtual_page_size_v;
	}

	void instance::MarkEntityAsMoved( const u32 MovedEntityPoolIndex ) noexcept
	{
		auto& MovedEntity     = GetComponent<paperback::component::entity>( MovedEntityPoolIndex );
		auto& MovedEntityInfo = m_pCoordinator->GetEntityInfo( MovedEntity );

		MovedEntity.m_Validation.m_bZombie = MovedEntityInfo.m_Validation.m_bZombie
										   = true;
		MovedEntityInfo.m_Validation.m_Next = m_MoveHead;
		m_MoveHead = MovedEntity.m_GlobalIndex;
	}

	void instance::AbandonPrefabInstancesOnPrefabDelete( const component::info& CInfo
													   , const u32 GlobalIndex ) noexcept
	{
		if ( CInfo.m_Guid == component::info_v<prefab>.m_Guid )
		{
			// Get Prefab Info & Prefab Component
			auto& PrefabInfo = m_pCoordinator->GetEntityInfo( GlobalIndex );
			auto& Prefab     = GetComponent<prefab>( PrefabInfo.m_PoolDetails.m_PoolIndex );

			if (Prefab.m_ReferencePrefabGIDs.size() != 0)
			{
				// Get Prefab Instance Archetype
				auto& PrefabInstanceArchetype = *( m_pCoordinator->GetEntityInfo(*(Prefab.m_ReferencePrefabGIDs.begin())).m_pArchetype );

				for ( auto begin = Prefab.m_ReferencePrefabGIDs.begin()
                    , end = Prefab.m_ReferencePrefabGIDs.end(); begin != end; ++begin )
				{
					const auto& InstanceInfo = m_pCoordinator->GetEntityInfo( *begin );

					m_pCoordinator->AddOrRemoveComponents< std::tuple<>
														 , std::tuple< reference_prefab > >
														 ( PrefabInstanceArchetype.GetComponent<paperback::component::entity>( InstanceInfo.m_PoolDetails ) );
				}
			}
		}
	}
}