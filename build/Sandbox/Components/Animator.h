/**********************************************************************************
*\file         Animator.h
*\brief        Animator.h
*
*\author       Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "../Functionality/Animation/Animation.h"

struct animator
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Animator"
	};

	animator()
	{
		for (int i = 0; i < 100; ++i)
		{
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
		}
	}

	std::vector<glm::mat4> m_FinalBoneMatrices;
	std::string m_CurrentAnimationName;
	float m_CurrentTime{};
	bool m_PlayOnce{};
	bool m_FinishedAnimating{false};
	bool m_PauseAnimation{};
	float m_PauseAtTime{-1};
	bool m_Reversed{};
};


namespace RR_Animator
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<animator>(animator::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Current Animation", &animator::m_CurrentAnimationName)(rttr::policy::prop::as_reference_wrapper)
			.property("Play Once", &animator::m_PlayOnce)(rttr::policy::prop::as_reference_wrapper)
			.property("Pause Animation", &animator::m_PauseAnimation)(rttr::policy::prop::as_reference_wrapper)
			.property("Current Time", &animator::m_CurrentTime)(rttr::policy::prop::as_reference_wrapper)
			.property("Pause Animation", &animator::m_PauseAnimation)(rttr::policy::prop::as_reference_wrapper)
			.property("Pause At Time", &animator::m_PauseAtTime)(rttr::policy::prop::as_reference_wrapper)
			.property("Reversed", &animator::m_Reversed)(rttr::policy::prop::as_reference_wrapper);
	}
}