/**********************************************************************************
*\file         paperback_settings.h
*\brief        paperback_settings.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace paperback::settings
{
	constexpr auto              max_component_types_v			    = 128;
	constexpr auto              max_components_per_entity_v		    = 64;
	constexpr auto              max_entities_per_pool_v			    = 5000;
	constexpr auto              max_entities_v					    = 5000;
	constexpr auto              max_partition_cells_v               = 128;
	constexpr auto              partition_cell_size_v               = 7;
	constexpr auto              batched_update_delay_v              = 5;
	constexpr auto              virtual_page_size_v				    = 4096;
	constexpr auto              invalid_index_v					    = UINT32_MAX;
	constexpr auto              invalid_controller_v                = GLFW_JOYSTICK_LAST + 1;
	constexpr auto              velocity_axis_cap_v                 = 2.6f;
	constexpr auto              momentum_axis_cap_v                 = 10.0f;
	constexpr auto              momentum_y_axis_cap_v				= 25.0f;
	constexpr auto              gamepad_axis_ignore_v               = 0.2f;
	constexpr auto              scene_transition_delay_v            = 0.5f;
	constexpr static u32        invalid_delete_index_v              = 0xffffffffu >> 1;
}