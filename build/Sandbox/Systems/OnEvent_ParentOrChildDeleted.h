/**********************************************************************************
*\file         OnEvent_ParentOrChildDeleted.h
*\brief        OnEvent_ParentOrChildDeleted.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct onevent_ParentDeleted_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "OnEvent_ParentDeleted"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass< paperback::vm::instance::OnEvent_ParentDeleted >( this );
    }

    void OnEvent( const parent& Parent ) noexcept
    {
		// Deletes children before parent, on deletion of parent
		auto ChildrenList = Parent.m_ChildrenGlobalIndexes;
		
		for ( const auto ChildGID : ChildrenList )
		{
			auto& ChildInfo = GetEntityInfo( ChildGID );
			if ( ChildInfo.m_pArchetype )
				ChildInfo.m_pArchetype->DestroyEntity( ChildInfo.m_pArchetype->GetComponent<paperback::component::entity>( ChildInfo.m_PoolDetails ) );
		}
    }
};


struct onevent_ChildDeleted_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::global_system_event
    {
        .m_pName = "OnEvent_ChildDeleted"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass< paperback::vm::instance::OnEvent_ChildDeleted >( this );
    }

    void OnEvent( const child& Child, const paperback::u32& GlobalIndex ) noexcept
    {
		// Get parent info
		const auto& ParentInfo = GetEntityInfo( Child.m_ParentGlobalIndex );

		auto& Parent = ParentInfo.m_pArchetype->GetComponent<parent>( ParentInfo.m_PoolDetails );
		Parent.RemoveChild( GlobalIndex );
    }
};