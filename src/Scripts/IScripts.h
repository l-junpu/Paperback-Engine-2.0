/**********************************************************************************
*\file         IScripts.h
*\brief        IScripts.h
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#include "Mono.h"
#include "paperback_coordinator.h"
#include "paperback_logger.h"

class Script
{
	// Mono Class Pointer
	MonoClass* m_pClass = nullptr;
	// Mono Object Instance
	MonoObject* m_pMonoObj = nullptr;
	// Mono Function Pointers
	MonoMethod* m_pStart = nullptr;
	MonoMethod* m_pPreUpdate = nullptr;
	MonoMethod* m_pUpdate = nullptr;
	MonoMethod* m_pDestroy = nullptr;

	MonoMethod* m_pOnCollisionEnter = nullptr;
	MonoMethod* m_pOnCollisionStay = nullptr;
	MonoMethod* m_pOnCollisionExit = nullptr;

	MonoMethod* m_pReset = nullptr;

	uint32_t m_Handle;

	// Class Name
	std::string m_ScriptClass;

public:

	void Init(std::string m_class)
	{
		m_ScriptClass = m_class;
		ScriptUpdate();
	}

	void ScriptUpdate()
	{
		std::string str;
		// Assign Class 
		m_pClass = Mono::GetInstanced().ImportClass("CSScript", m_ScriptClass.c_str());

		if (m_pClass) {

			str = "." + m_ScriptClass + ":getInst()";
			m_pMonoObj = Mono::GetInstanced().GetClassInstance(str.c_str(), m_pClass, &m_Handle);

			if (m_pMonoObj) {

				str = "." + m_ScriptClass + ":Start(uint)";
				m_pStart = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":PreUpdate(single)";
				m_pPreUpdate = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":Update(single)";
				m_pUpdate = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":Destroy()";
				m_pDestroy = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":OnCollisionEnter(uint)";
				m_pOnCollisionEnter = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":OnCollisionStay(uint)";
				m_pOnCollisionStay = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":OnCollisionExit(uint)";
				m_pOnCollisionExit = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());

				str = "." + m_ScriptClass + ":Reset()";
				m_pReset = Mono::GetInstanced().ImportFunction(m_pClass, m_pMonoObj, str.c_str());
			}
		}
	}

	void ChangeScene()
	{
		mono_gchandle_free(m_Handle);
	}

	void Start(uint32_t ID)
	{
		if (m_pStart)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pStart, ID);
	}

	void PreUpdate(float dt)
	{
		if (m_pPreUpdate)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pPreUpdate, dt);
	}

	void Update(float dt)
	{
		if (m_pUpdate)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pUpdate, dt);
	}

	void Destroy()
	{
		if (m_pDestroy)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pDestroy);
	}

	void OnCollisionEnter(uint32_t ID)
	{
		if (m_pOnCollisionEnter)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pOnCollisionEnter, ID);
	}

	void OnCollisionStay(uint32_t ID)
	{
		if (m_pOnCollisionStay)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pOnCollisionStay, ID);
	}

	void OnCollisionExit(uint32_t ID)
	{
		if (m_pOnCollisionExit)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pOnCollisionExit, ID);
	}

	void Reset()
	{
		if (m_pReset)
			Mono::GetInstanced().RunImportFn(m_pMonoObj, m_pReset);
	}
};
