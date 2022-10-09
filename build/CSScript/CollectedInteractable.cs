﻿/**********************************************************************************
*\file         CollectedInteractable.cs
*\brief        CollectedInteractable.cs Script
*
*\author	     Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class CollectedInteractable : MonoBehaviour
    {
        UInt32 m_InteractableID;                    // Interactable Entity ID
        Sound  m_InteractableSound;                 // Interactable Sound Component
        AbilityCollectible m_InteractableAbility;   // Interactable Ability Component
        Mesh m_InteractableMesh;                    // Interactable Mesh Component
        ParticleEmitter m_InteractableEmitter;      // Interactable Particle Emitter Component

        Int32 m_JumpID;                             // Jump Unit ID
        Int32 m_PushID;                             // Push Unit ID
                                                    
        PlayerController m_JumpUnitController;      // Jump Unit Player Controller Component
        Camera           m_JumpUnitCamera;          // Jump Unit Camera Component
                                                    
        PlayerController m_PushUnitController;      // Push Unit Player Controller Component
        Camera           m_PushUnitCamera;          // Push Unit Camera Component

        bool m_Activated;

        public static CollectedInteractable getInst()
        {
            return new CollectedInteractable();
        }

        public void Start( UInt32 InteractableID )
        {
            m_InteractableID      = InteractableID;
            m_InteractableSound   = new Sound( m_InteractableID );
            m_InteractableAbility = new AbilityCollectible( m_InteractableID );
            m_InteractableMesh    = new Mesh(m_InteractableID);
            m_InteractableEmitter = new ParticleEmitter(m_InteractableID);

            m_JumpID = Player.GetJumpUnitID();
            m_PushID = Player.GetPushUnitID();

            m_JumpUnitController = new PlayerController( (UInt32)m_JumpID );
            m_PushUnitController = new PlayerController( (UInt32)m_PushID );
            m_JumpUnitCamera     = new Camera( (UInt32)m_JumpID );
            m_PushUnitCamera     = new Camera( (UInt32)m_PushID );

            m_InteractableSound.m_Trigger = false;
            m_Activated = false;
        }

        public void PreUpdate(float dt)
        {
            Application.NotifyDone();
        }

        public void Update( float dt )
        {
            Application.NotifyDone();

        }

        public void Destroy()
        {

        }

        public void OnCollisionEnter( UInt32 ID )
        {
            if (m_InteractableSound != null && m_InteractableAbility != null && m_InteractableMesh != null && m_InteractableEmitter != null &&
                m_JumpUnitController != null && m_JumpUnitCamera != null && m_PushUnitController != null && m_PushUnitCamera != null)
            {
                if (!m_Activated && m_JumpID != -1 && m_PushID != -1)
                {
                    if (ID == m_JumpID || ID == m_PushID)
                    {
                        if (m_InteractableAbility.m_Grow)
                        {
                            m_JumpUnitController.m_GrowAvailable = true;
                            m_PushUnitController.m_GrowAvailable = true;
                            m_InteractableAbility.m_Grow = false;
                        }
                        else if (m_InteractableAbility.m_Shrink)
                        {
                            m_JumpUnitController.m_ShrinkAvailable = true;
                            m_PushUnitController.m_ShrinkAvailable = true;
                            m_InteractableAbility.m_Shrink = false;
                        }
                        else if (m_InteractableAbility.m_Freeze)
                        {
                            m_JumpUnitController.m_FreezeAvailable = true;
                            m_PushUnitController.m_FreezeAvailable = true;
                            m_InteractableAbility.m_Freeze = false;
                        }

                        m_Activated = true;
                        m_InteractableSound.m_Trigger = true;
                        m_InteractableMesh.m_Active = false;
                        m_InteractableEmitter.m_Lifetime = 0.0f;
                    }
                }
            }

            Application.NotifyDone();
        }

        public void OnCollisionStay(UInt32 ID)
        {
            Application.NotifyDone();
        }

        public void OnCollisionExit(UInt32 ID)
        {
            Application.NotifyDone();
        }
        public void Reset()
        {
        }
    }
}
