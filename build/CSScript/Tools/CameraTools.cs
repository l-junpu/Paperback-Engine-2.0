﻿/**********************************************************************************
*\file         CameraTools.cs
*\brief        CameraTools.cs Script
*
*\author	     Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript.Tools
{
    public class CameraTools
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveForward();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveBackward();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveLeft();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveRight();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveUp();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveDown();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateUp();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateDown();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateLeft();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateRight();
    }
}
