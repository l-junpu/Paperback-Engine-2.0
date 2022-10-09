/**********************************************************************************
*\file         PropertyInspector.h
*\brief        PropertyInspector.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Editor/EditorInclude.h"

struct DetailsWindow : paperback::editor::instance
{
    bool ComboUpdated = false;

    constexpr static auto typedef_v = paperback::editor::type::update
    {
        .m_pName = "Property Inspector"
    };

    void OnSystemCreated(void) noexcept
    {
        m_bEnabled = true;
    }

    void Update(void) noexcept
    {
        Panel();
    }

    void Panel();

    void DisplayProperties();

    void AddComponent();

    void RemoveComponent();

    bool PrefabRefComboCheck(std::string& PreviousEntry, std::string& NewEntry);

//-----------------------------------
//           Components
//-----------------------------------

    void ParentComponent( prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

    void AnimatorComponent( paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

    void SocketedComponent( paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

    void PrefabComponent();

    void PathComponent(); // For Spline 

    void DeckComponent();

    void CardComponent( paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

    void ButtonComponent( paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

    void EntityScriptComponent();

    void CinematicComponent();

//-----------------------------------
//        Component Combos
//-----------------------------------
    void ChildCombo( prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

    void MeshCombo(paperback::entity::info& EntityInfo, prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

    void DisplayAvailableChildren( paperback::component::entity& Entity, paperback::u32 i,
        parent& NewParent, paperback::component::entity NewParentEntity,
        prefab* Prefab, reference_prefab* ReferencePrefab, const paperback::component::type::guid CompGuid );

};