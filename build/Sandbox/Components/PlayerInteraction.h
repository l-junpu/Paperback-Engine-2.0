/**********************************************************************************
*\file         PlayerInteraction.h
*\brief        PlayerInteraction.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct player_interaction
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Player Interaction"
	};

	paperback::u32        m_InteractableGID = paperback::settings::invalid_index_v;
	bool                  m_bPushOrPull = false;
};

namespace RR_PLAYER_INTERACTION
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<player_interaction>( player_interaction::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
			.property( "Interactable ID", &player_interaction::m_InteractableGID )
			.property( "Pushing Or Pulling?", &player_interaction::m_bPushOrPull )(rttr::policy::prop::as_reference_wrapper );
	}
}