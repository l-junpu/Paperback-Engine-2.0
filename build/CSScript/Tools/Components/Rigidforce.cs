﻿/**********************************************************************************
*\file         Rigidforce.cs
*\brief        Rigidforce.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
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

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public unsafe class Rigidforce
    {
        private void* m_Address;

        public Rigidforce(UInt32 id)
        {
            m_Address = getaddress(id);
        }

        public float m_StaticFriction
        {
            get
            {
                return getstaticfriction(m_Address);
            }
            set
            {
                setstaticfriction(m_Address, value);
            }
        }

        public float m_DynamicFriction
        {
            get
            {
                return getdynamicfriction(m_Address);
            }
            set
            {
                setdynamicfriction(m_Address, value);
            }
        }

        public Tools.MathLib.Vector3 m_Forces
        {
            get
            {
                return getforces(m_Address);
            }
            set
            {
                setforces(m_Address, value.x, value.y, value.z);
            }
        }

        public Tools.MathLib.Vector3 m_Momentum
        {
            get
            {
                return getmomentum(m_Address);
            }
            set
            {
                setmomentum(m_Address, value.x, value.y, value.z);
            }
        }

        public float m_Restitution
        {
            get
            {
                return getrestitution(m_Address);
            }
            set
            {
                setrestitution(m_Address, value);
            }
        }

        public bool m_CollisionAffected
        {
            get
            {
                return getcollisionaffected(m_Address);
            }
            set
            {
                setcollisionaffected(m_Address, value);
            }
        }

        public bool m_GravityAffected
        {
            get
            {
                return getgravityaffected(m_Address);
            }
            set
            {
                setgravityaffected(m_Address, value);
            }
        }
        public bool m_GravityActive
        {
            get
            {
                return getgravityactive(m_Address);
            }
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void* getaddress(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getstaticfriction(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setstaticfriction(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getdynamicfriction(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setdynamicfriction(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getforces(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setforces(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Tools.MathLib.Vector3 getmomentum(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setmomentum(void* address, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getrestitution(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setrestitution(void* address, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getgravityaffected(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setgravityaffected(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getcollisionaffected(void* address);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setcollisionaffected(void* address, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool getgravityactive(void* address);
    }
}
