/**********************************************************************************
*\file         MouseSens_Decrease_Button_MainMenu.h
*\brief        MouseSens_Decrease_Button_MainMenu.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct mousesens_decrease_button_mainmenu_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
    {
        .m_pName = "Mouse Sensitivity Decrease Button MainMenu Script"
    };

    // You have to define this constructor - For internal registration
    mousesens_decrease_button_mainmenu_script(paperback::coordinator::instance& Instance) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover(void) noexcept override
    {
    }

    void OnClick() noexcept override
    {

        PPB.GetSystem<sound_system>().TriggerTaggedSound("ButtonClickSFX");

        float sens = PPB.GetMouseSensitivityRatio();

        if (sens > 0.2f)
        {

            sens -= 0.2f;
            PPB.SetMouseSensitivityRatio(sens);
        }

        for (size_t layer = UI_LAYER::MOUSESENS1; layer <= UI_LAYER::MOUSESENS5; ++layer)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer), false);
        }

        size_t layer = UI_LAYER::MOUSESENS1;

        for (float mousesens = 0.0f; mousesens < sens; mousesens += 0.2f)
        {

            PPB.GetSystem<ui_system>().ToggleLayerObjects(static_cast<int>(layer++), true);
        }
    }
};