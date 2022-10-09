/**********************************************************************************
*\file         Scripts_Pushable.h
*\brief        Scripts_Pushable.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Mono.h"

namespace MONO_PUSHABLE
{
	MONO_EXPORT void* GetAddress(uint32_t ID)
	{
		auto m_obj = PPB.GetEntityInfo(ID);
		void* m_pushable = m_obj.m_pArchetype->FindComponent<pushable>(m_obj.m_PoolDetails);

#ifdef PAPERBACK_DEBUG
		if (!m_pushable)
		{
			name* Name = m_obj.m_pArchetype->FindComponent<name>(m_obj.m_PoolDetails);
			std::cout << "Object with ID " + std::to_string(ID) + " and name " + Name->m_Value + " has no Pushable component." << std::endl;
		}
#endif

		return m_pushable;
	}

	MONO_EXPORT int32_t GetState(void* address)
	{
		if (address)
			return reinterpret_cast<pushable*>(address)->m_State;

		return {};
	}

	MONO_EXPORT void SetState(void* address, int32_t state)
	{
		if (address)
			reinterpret_cast<pushable*>(address)->m_State = state;
	}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Pushable::getaddress(uint)", &MONO_PUSHABLE::GetAddress);
		mono_add_internal_call("CSScript.Pushable::getstate(void*)", &MONO_PUSHABLE::GetState);
		mono_add_internal_call("CSScript.Pushable::setstate(void*,int)", &MONO_PUSHABLE::SetState);
	}
}