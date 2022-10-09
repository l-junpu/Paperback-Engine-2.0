/**********************************************************************************
*\file         paperback_component_inline.h
*\brief        paperback_component_inline.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace paperback::component
{
    //-----------------------------------
    //        Component Info
    //-----------------------------------
    namespace details
    {
        template< typename T_COMPONENT >
        consteval info CreateInfo( void ) noexcept
        {
            return info
            {
                .m_Guid               = std::is_same_v< component::entity, T_COMPONENT >
                                        ? type::guid{ nullptr }
                                        : T_COMPONENT::typedef_v.m_Guid.m_Value
                                            ? T_COMPONENT::typedef_v.m_Guid
                                            : type::guid{ __FUNCSIG__ }
            ,   .m_TypeID             = T_COMPONENT::typedef_v.id_v
            ,   .m_UID                = std::is_same_v<T_COMPONENT, paperback::component::entity> ? 0 : info::invalid_id_v
            ,   .m_Size               = static_cast<uint32_t>( sizeof(T_COMPONENT) )
            ,   .m_Constructor        = std::is_trivially_constructible_v<T_COMPONENT>
                                        ? nullptr
                                        : []( std::byte* Ptr ) noexcept
                                          {
                                              new( Ptr ) T_COMPONENT;
                                          }
            ,   .m_Destructor         = std::is_trivially_destructible_v<T_COMPONENT>
                                        ? nullptr
                                        : []( std::byte* Ptr ) noexcept
                                          {
                                              std::destroy_at( reinterpret_cast<T_COMPONENT*>( Ptr ) );
                                          }
            ,   .m_UpdateInstances    = []( std::byte* Ptr, const vm::PoolDetails Details, paperback::archetype::instance* Prefab ) noexcept
                                        {
                                            T_COMPONENT& C = *( reinterpret_cast<T_COMPONENT*>( Ptr ) );
                                            Prefab->UpdatePrefabInstanceComponent( Details, C );
                                        }
            ,   .m_Copy               = std::is_trivially_copy_assignable_v<T_COMPONENT>
                                        ? nullptr
                                        : []( std::byte* Destination, const std::byte* Source ) noexcept
                                        {
                                            *reinterpret_cast<T_COMPONENT*>( Destination ) = *reinterpret_cast<const T_COMPONENT*>( Source );
                                        }
            ,   .m_Move               = std::is_trivially_move_assignable_v<T_COMPONENT>
                                        ? nullptr
                                        : []( std::byte* Destination, std::byte* Source ) noexcept
                                          {
                                              *reinterpret_cast<T_COMPONENT*>( Destination ) = std::move( *reinterpret_cast<T_COMPONENT*>( Source ) );
                                          }
            ,   .m_pName              = T_COMPONENT::typedef_v.m_pName
            };
        }
        //-----------------------------------
        //        Component Sorting
        //-----------------------------------
        template< typename T_COMPONENT_A, typename T_COMPONENT_B >
        struct component_comparator
        {
            constexpr static bool value = component::info_v< T_COMPONENT_A >.m_Guid
                                        < component::info_v< T_COMPONENT_B >.m_Guid;
        };

        template< paperback::concepts::TupleSpecialization T_TUPLE >
        using sort_tuple_t = xcore::types::tuple_sort_t< details::component_comparator, T_TUPLE >;


        //-----------------------------------
        //           Find Index
        //-----------------------------------
        template < typename T_CONTAINER >
        static constexpr auto find_component_index_v = [&]( T_CONTAINER& Container, const component::info* Info, int Count )
        {
            return static_cast<size_t>( std::upper_bound( std::begin( Container )
                                                        , std::begin( Container ) + Count
                                                        , Info
                                                        , []( const component::info* pA, const component::info* pB)
                                                          {
                                                              return pA->m_UID < pB->m_UID;
                                                          }
                                                        ) - std::begin( Container )
                                      );
        };
    }

    //-----------------------------------
    //   Get array of Component Info
    //-----------------------------------
    template< paperback::concepts::TupleSpecialization T_TUPLE >
    static constexpr auto sorted_info_array_v = []<typename...T>( std::tuple<T...>* ) constexpr
    {
        if constexpr ( sizeof...(T) == 0 ) return std::span<const component::info*>{};
        else                               return std::array{ &component::info_v<T>... };
    }( xcore::types::null_tuple_v< details::sort_tuple_t<T_TUPLE> > );


    //-----------------------------------
    //        Component: Entity
    //-----------------------------------
    constexpr bool entity::Validation::operator == ( const Validation& V ) const noexcept
    {
        return m_UID == V.m_UID;
    }
    
    constexpr bool entity::IsZombie( void ) const noexcept
    {
        return m_Validation.m_bZombie;
    }

    constexpr bool entity::operator == ( const entity& Entity ) const noexcept
    {
        return m_GlobalIndex == Entity.m_GlobalIndex;
    }
}