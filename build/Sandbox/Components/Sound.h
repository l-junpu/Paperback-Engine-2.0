/**********************************************************************************
*\file         Sound.h
*\brief        Sound.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct sound
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Sound"
    };

	std::string m_SoundID; //contains the path/id of the event to be played from the bank
    size_t m_SoundPlayTag; // assigned during run time to determine if currently playing
    bool m_Is3DSound; //set to false if 2D, true if 3D
    bool m_IsTriggerable = false; //set to false by default, assumes that sound is able to be played again whenever required
    bool m_Trigger = false; // accomnpanying bool to IsTriggerable to indicate if sound is to be played
    bool m_ForceStop = false;
};

namespace RR_Sound
{
    RTTR_REGISTRATION
    {
         rttr::registration::class_<sound>(sound::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           .property("Path", &sound::m_SoundID)
           .property("Sound Event ID", &sound::m_SoundPlayTag)
           .property("3D Sound", &sound::m_Is3DSound)(rttr::policy::prop::as_reference_wrapper)
           .property("Triggerable", &sound::m_IsTriggerable)(rttr::policy::prop::as_reference_wrapper)
           .property("Trigger", &sound::m_Trigger);
    }
}