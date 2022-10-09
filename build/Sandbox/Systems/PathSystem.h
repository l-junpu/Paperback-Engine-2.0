/**********************************************************************************
*\file         PathSystem.h
*\brief        PathSystem.h
*
*\author       Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef _PATHSYSTEM_H_
#define _PATHSYSTEM_H_

#define _USE_MATH_DEFINES

#include "DebugSystem.h"
#include "UISystem.h"
#include "../Components/Path.h"
#include "Math/Math_includes.h"
#include <math.h>
#include <algorithm>
#include <map>

struct path_system : paperback::system::instance
{
	// System Ptrs
	debug_system* debug_sys;

	// Queries
	tools::query Query_Paths;
	tools::query Query_PathFollowers;
	tools::query Query_CinematicPathFollowers;

	bool m_MovePathFollowers{};

	// Map of splines
	std::map<int, paperback::Spline> splines;

	constexpr static auto typedef_v = paperback::system::type::update
	{
		.m_pName = "path_system"
	};

	PPB_FORCEINLINE
	void OnSystemCreated(void) noexcept
	{
		debug_sys = &GetSystem<debug_system>();

		Query_Paths.m_Must.AddFromComponents<path, transform>();
		Query_Paths.m_NoneOf.AddFromComponents<prefab>();

		Query_PathFollowers.m_Must.AddFromComponents<path_follower, transform>();
		Query_PathFollowers.m_OneOf.AddFromComponents<name, offset>();
		Query_PathFollowers.m_NoneOf.AddFromComponents<prefab, cinematic>();

		Query_CinematicPathFollowers.m_Must.AddFromComponents<path_follower, transform, cinematic>();
		Query_CinematicPathFollowers.m_OneOf.AddFromComponents<name, offset>();
		Query_CinematicPathFollowers.m_NoneOf.AddFromComponents<prefab>();
	}

	PPB_INLINE
	void OnStateChange( void ) noexcept
	{
		//splines.clear();
	}

	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
		/*ForEach(Search(Query_Paths), [&]( path& Path, transform& Transform ) noexcept
		{
			std::vector<paperback::Spline::SplinePoint> spline_points;
		
			for (auto& point : Path.m_Points)
			{
				spline_points.push_back({ Transform.m_Position + point });
			}
		
			splines.emplace(Path.m_ID, paperback::Spline{ spline_points, false });
		});*/
	}

	PPB_FORCEINLINE
		void OnPause(const bool& Status) noexcept	//function activates whenever play / pause is clicked
	{
		m_bPaused = Status;

		if (!Status) //is paused, play is clicked
		{
			m_MovePathFollowers = true;
		}

		else
		{
			m_MovePathFollowers = false;
		}
	}

	PPB_FORCEINLINE
	void Update(void) noexcept
	{
		ForEach(Search(Query_Paths), [&](path& Path, transform& Transform) noexcept
		{
			std::vector<paperback::Spline::SplinePoint> spline_points;

			for (auto& point : Path.m_Points)
			{
				spline_points.push_back({ Transform.m_Position + point });
			}

			splines.emplace(Path.m_ID, paperback::Spline{ spline_points, false });
		});

		ForEach(Search(Query_Paths), [&]( path& Path ) noexcept
		{
			if (debug_sys->m_IsDebug)
			{
				debug_sys->DrawSpline(splines[Path.m_ID]);
			}
		});

		if (m_MovePathFollowers)
		{
			ForEach(Search(Query_PathFollowers), [&](path_follower& PathFollower, transform& Transform, offset* Offset) noexcept
			{
				auto spline = splines.find(PathFollower.m_PathID);

				if (spline != splines.end() && !PathFollower.m_PauseTravel)
				{
					if ((!PathFollower.m_Reversed && PathFollower.m_Distance >= spline->second.m_TotalLength ||
						 PathFollower.m_Reversed && PathFollower.m_Distance <= 0.0f))
					{
						PathFollower.m_FinishedTravelling = true;

						if (PathFollower.m_BackAndForth)
						{
							PathFollower.m_Reversed = !PathFollower.m_Reversed;
							Movement(spline->second, PathFollower, Transform, Offset);
						}
					}

					else
					{
						PathFollower.m_FinishedTravelling = false;
						Movement(spline->second, PathFollower, Transform, Offset);
					}
				}
			});

			ForEach(Search(Query_CinematicPathFollowers), [&](path_follower& PathFollower, transform& Transform, cinematic& Cinematic, offset* Offset) noexcept
			{
				auto spline = splines.find(PathFollower.m_PathID);

				if (spline != splines.end() && !PathFollower.m_PauseTravel)
				{
					if (!PathFollower.m_Reversed && PathFollower.m_Distance >= spline->second.m_TotalLength)
					{
						PathFollower.m_FinishedTravelling = true;
					}

					else
					{
						CinematicMovement(spline->second, PathFollower, Transform, Cinematic, Offset);
					}
				}
			});
		}

		splines.clear();
	}

	void Movement(paperback::Spline& spline, path_follower& PathFollower, transform& Transform, offset* Offset)
	{
		float normalized_offset{ spline.GetNormalizedOffset(PathFollower.m_Distance) };

		paperback::Vector3f destination{ spline.GetSplinePoint(normalized_offset).m_Point };
		paperback::Vector3f direction{ (destination - Transform.m_Position) };
		float speed_modifier = 1.7f + 0.8f * cosf(PathFollower.m_Distance / spline.m_TotalLength * 2 * static_cast<float>(M_PI) + static_cast<float>(M_PI));

		if (Offset)
		{
			Offset->m_PosOffset += direction;
		}

		else
		{
			Transform.m_Position += direction;
		}

		if (PathFollower.m_Reversed)
		{
			PathFollower.m_Distance -= speed_modifier * PathFollower.m_TravelSpeed * m_Coordinator.DeltaTime();

			if (PathFollower.m_Distance < 0.0f)
			{
				PathFollower.m_Distance = 0.00f;
			}
		}

		else
		{
			PathFollower.m_Distance += speed_modifier * PathFollower.m_TravelSpeed * m_Coordinator.DeltaTime();


			if (PathFollower.m_Distance > spline.m_TotalLength)
			{
				PathFollower.m_Distance = spline.m_TotalLength;
			}
		}

		std::vector<paperback::Vector3f> vec;
		vec.push_back(destination);
		vec.push_back(Transform.m_Position);
		
		if (debug_sys->m_IsDebug)
		{
			debug_sys->DrawDebugLines(vec, true);
		}
	}

	void CinematicMovement(paperback::Spline& spline, path_follower& PathFollower, transform& Transform, cinematic& Cinematic, offset* Offset)
	{
		if (Cinematic.m_CinematicInfos.size() > Cinematic.m_Index)
		{
			cinematic::CinematicInfo& cinematic_info{ Cinematic.m_CinematicInfos[Cinematic.m_Index] };

			if (Cinematic.m_OnHold)
			{
				Cinematic.m_Timer += m_Coordinator.DeltaTime();

				if (Cinematic.m_Timer > cinematic_info.m_HoldTime)
				{
					Cinematic.m_OnHold = false;
					Cinematic.m_Timer = 0.0f;
					++Cinematic.m_Index;
				}
			}

			else //moving
			{
				float normalized_offset{ spline.GetNormalizedOffset(PathFollower.m_Distance) };

				if (normalized_offset > Cinematic.m_Index)
				{
					normalized_offset = static_cast<float>(Cinematic.m_Index);
					Cinematic.m_OnHold = true;
				}

				paperback::Vector3f destination{ spline.GetSplinePoint(normalized_offset).m_Point };
				paperback::Vector3f direction{ (destination - Transform.m_Position) };
				
				float prev_dist{0.0f};

				for (int i = 0; i < Cinematic.m_Index - 1; ++i)
				{
					prev_dist += spline.CalculateSegmentLength(i);
				}

				float destination_dist{ spline.CalculateSegmentLength(Cinematic.m_Index) };
				float speed_modifier = static_cast<float>(1.7f + 0.8f * cosf((PathFollower.m_Distance - prev_dist) / (destination_dist - prev_dist) * 2 * static_cast<float>(M_PI) + static_cast<float>(M_PI)));

				if (Offset)
				{
					Offset->m_PosOffset += direction;
				}

				else
				{
					Transform.m_Position += direction;
				}

				PathFollower.m_Distance += speed_modifier * cinematic_info.m_MoveSpeed * m_Coordinator.DeltaTime();

				if (PathFollower.m_Distance > spline.m_TotalLength)
				{
					PathFollower.m_Distance = spline.m_TotalLength;
				}
			}
		}
	}

	paperback::Vector3f GetRotationAngles(paperback::Vector3f vec1, paperback::Vector3f vec2)
	{
		paperback::Vector3f axis_of_rot{ vec1.Cross(vec2) };
		float angle_of_rot{ vec1.Angle(vec2) };

		if (axis_of_rot == paperback::Vector3f{} || axis_of_rot == paperback::Vector3f{180.0f, 180.0f, 180.0f} || angle_of_rot == 0.0f || std::isnan(angle_of_rot))
		{
			return {};
		}

		float qw{ cos(angle_of_rot / 2.0f) };
		float qx{ axis_of_rot.x * sin(angle_of_rot / 2.0f) };
		float qy{ axis_of_rot.y * sin(angle_of_rot / 2.0f) };
		float qz{ axis_of_rot.z * sin(angle_of_rot / 2.0f) };

		float heading{ atan2(2 * qy * qw - 2 * qx * qz, 1 - 2 * qy * qy - 2 * qz * qz) };
		float attitude{ asin(2 * qx * qy + 2 * qz * qw) };
		float bank{ atan2(2 * qx * qw - 2 * qy * qz, 1 - 2 * qx * qx - 2 * qz * qz) };

		return { {std::isnan(heading) ? 0.0f : heading},
				 {std::isnan(bank) ? 0.0f : bank},
				 {std::isnan(attitude) ? 0.0f : attitude } };
	}

//	void OnEvent(const size_t& Key, const bool& Clicked) noexcept
//	{
//		if (!Clicked && Key == GLFW_MOUSE_BUTTON_1 && ui_sys->m_Picked)
//		{
//			//lane selection
//			struct lane_box
//			{
//				paperback::Vector3f m_Min;
//				paperback::Vector3f m_Max;
//				paperback::Vector3f m_Position;
//				int m_Lane;
//			};
//
//			std::vector<lane_box> lane_boxes;
//
//			for (int i = 0 ; i < splines.size() ; ++i)
//			{
//				float interval{ 3.0f };
//
//				for (float normalized_offset = 0.0f, distance = 0.0f;
//					 normalized_offset < static_cast<float>(splines[i].m_Points.size()) - 3.5f;
//					 distance += interval)
//				{
//					paperback::Vector3f box_point{ splines[i].GetSplinePoint(normalized_offset).m_Point };
//					paperback::Vector3f min{ -2.3f, -2.3f, -2.3f };
//					paperback::Vector3f max{ -min };
//					lane_boxes.push_back({ min, max, box_point, i });
//					normalized_offset = splines[i].GetNormalizedOffset(distance);
//				}
//			}
//
//			int lane{-1};
//			glm::vec3 CamPos{ cam::GetInstanced().GetPosition() };
//			glm::vec3 RayDir{ PPB.GetMousePosition() };
//
//			for (auto lane_box : lane_boxes)
//			{
//				float t = 0.0f;
//
//				if (RayAabb({ CamPos.x, CamPos.y, CamPos.z },
//							{ RayDir.x, RayDir.y, RayDir.z },
//							lane_box.m_Position + lane_box.m_Min, lane_box.m_Position + lane_box.m_Max, t))
//				{
//					lane = lane_box.m_Lane;
//					break;
//				}
//			}
//
//			ForEach(Search(Query_Paths), [&]( path& Path, selected& Selected, mesh& Mesh ) noexcept
//			{
//				if (Path.m_ID == lane)
//				{
//					Selected.m_Value = true;
//					Mesh.m_Active = true;
//				}
//
//				else
//				{
//					Selected.m_Value = false;
//					Mesh.m_Active = false;
//				}
//			});
//
//			if (debug_sys->m_IsDebug)
//			{
//				for (auto& lane_box : lane_boxes)
//				{
//					if (lane_box.m_Lane == lane)
//					{
//						boundingbox cube{ lane_box.m_Min, lane_box.m_Max, true };
//						debug_sys->DrawCube(cube, lane_box.m_Position);
//					}
//
//					else
//					{
//						boundingbox cube{ lane_box.m_Min, lane_box.m_Max, false };
//						debug_sys->DrawCube(cube, lane_box.m_Position);
//					}
//				}
//			}
//		}
//
//		else if (Clicked)
//		{
//			ForEach(Search(Query_Paths), [&]( selected& Selected, mesh& Mesh ) noexcept
//			{
//				Selected.m_Value = false;
//				Mesh.m_Active = false;
//			});
//		}
//	}
};

#endif
