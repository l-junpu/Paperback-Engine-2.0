/**********************************************************************************
*\file         paperback_mouse.h
*\brief        paperback_mouse.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#ifndef PPB_MOUSE_CONTROLS
#define PPB_MOUSE_CONTROLS

namespace paperback::input::device
{
    BEGIN_CONTROLLER_CONSTRUCT( Mouse_Controls
                              , device::type::mouse )

        PPB_INLINE
        void Update( const float Dt ) noexcept override
        {
            
        }

        PPB_INLINE
        void Reset( void ) noexcept
        {
            
        }    

        MouseState m_State;

    END_CONTROLLER_CONSTRUCT
}

#endif