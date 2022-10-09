﻿/**********************************************************************************
*\file         SplashScreenCollider.cs
*\brief        SplashScreenCollider.cs Script
*
*\author	     Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    class SplashScreenCollider
    {
        UInt32 m_ID;

        BoundingBox m_BoundingBox;
        public static SplashScreenCollider getInst()
        {
            return new SplashScreenCollider();
        }

        public void Start(UInt32 ID)
        {
            m_ID = ID;
            m_BoundingBox = new BoundingBox(m_ID);
        }

        public void PreUpdate(float dt)
        {

            Application.NotifyDone();
        }

        public void Update(float dt)
        {

            Application.NotifyDone();
        }

        public void Destroy()
        {
        }

        public void OnCollisionEnter(UInt32 ID)
        {
            if (m_BoundingBox != null)
            {
                Application.ChangeScene("MainMenu");
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
