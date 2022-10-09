/**********************************************************************************
*\file         IconSystem.h
*\brief        IconSystem.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Systems/DebugSystem.h"

struct icon_system : paperback::system::pausable_instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "icon_system"
    };

    // Initialize Query
    tools::query m_Query;

    using query = std::tuple<
        paperback::query::must<entity, icon, offset>
        , paperback::query::none_of<prefab>
    >;

    PPB_FORCEINLINE
        void OnSystemCreated(void) noexcept
    {
    }

    void operator()(paperback::component::entity& Entity, icon& Icon, offset& Offset) noexcept
    {
        if (Entity.IsZombie()) return;

        if (Offset.m_RotOffset.y > 360.f)
            Offset.m_RotOffset.y = 0.f;
        else {
            Offset.m_RotOffset.y += 1.f;
        }
    }
};