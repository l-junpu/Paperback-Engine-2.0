/**********************************************************************************
*\file         Viewport_Inline.h
*\brief        Viewport_Inline.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Viewport.h"
#include "glm/inc/gtc/type_ptr.hpp"
#include "glm/inc/gtx/matrix_decompose.hpp"
#include "../../Physics/geometry.h"

void EditorViewport::Panel()
{
	ViewportMenuBar();
	ViewportTwo();
	ViewportOne();
}

void EditorViewport::ViewportOne()
{
	//Renders the game

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport #1", &m_bEnabled);

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

	paperback::u64 TextureID = Renderer::GetInstanced().GetFinalImage();

	ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	Gizmo();
	MouseSelection();

	ImGui::End();
	ImGui::PopStyleVar();

}

void EditorViewport::ViewportTwo()
{
	// Renders the UI

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport #2", &m_bEnabled);

	ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

	paperback::u64 TextureID = Renderer::GetInstanced().GetUIOverlay();

	ImGui::Image(reinterpret_cast<void*>(TextureID), ViewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	Gizmo();
	MouseSelection();

	ImGui::End();
	ImGui::PopStyleVar();
}

void EditorViewport::ViewportMenuBar()
{
	std::string PlayIcon{}, DebugIcon{};
	std::stringstream ss;

	ImGui::Begin("##uitoolbar", nullptr, ImGuiWindowFlags_NoDecoration);

	if (ImGui::Button(ICON_FA_ARROWS_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;	
	}

	ImGui::SameLine(); 

	if (ImGui::Button(ICON_FA_EXPAND_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
	}
	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_SYNC_ALT))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_BROOM))
	{
		if (!ImGuizmo::IsUsing())
			m_GizmoType = -1;
	}

	ImGui::SameLine();

	std::reference_wrapper<bool> DebugBool = PPB.GetSystem< debug_system >().m_IsDebug;

	ImGui::Checkbox(ICON_FA_WRENCH, &DebugBool.get());

	m_Imgui.ImGuiHelp("Toggle Debug Lines");

	ImGui::SameLine();

	std::reference_wrapper<bool> GammaBool = PPB.GetSystem< render_system >().m_bGamma;
	ImGui::Checkbox(ICON_FA_LIGHTBULB, &GammaBool.get());

	m_Imgui.ImGuiHelp("Toggle Gamma");

	ImGui::SameLine();

	if (ImGui::Button(ICON_FA_RETWEET))
	{
		Mono::GetInstanced().UpdateDLL();
	}

	m_Imgui.ImGuiHelp("Toggle Hot Reloading");

	ImGui::SameLine();

	ImGui::Text(" | ");

	ImGui::SameLine();

	if (m_Imgui.m_bPaused)
		PlayIcon = ICON_FA_PLAY;
	else
		PlayIcon = ICON_FA_STOP;

	if (ImGui::Button(PlayIcon.c_str()))
	{
		if (m_Imgui.m_bPaused)
			m_Imgui.m_Type = FileActivity::PLAYBUTTON;
		else
			m_Imgui.m_Type = FileActivity::STOPBUTTON;
	}

	m_Imgui.ImGuiHelp("Play/Stop Current Scene");

	ImGui::Text("Adjust Camera Speed: "); ImGui::SameLine();

	ImGui::PushItemWidth(150.0f);

	std::reference_wrapper<float> CamSpeed = PPB.GetSystem< render_system >().m_Speed;
	ImGui::SliderFloat("##CameraSpeed", &(CamSpeed.get()), 0.1f, 10.0f);

	ImGui::PopItemWidth();

	m_Imgui.ImGuiHelp("Modify Camera Speed | Ctrl + Click to type");

	ImGui::End();
}

void EditorViewport::MouseSelection()
{
	if (PPB.IsMousePress(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (ImGui::IsWindowHovered() && !ImGuizmo::IsOver())
		{
			if (!PPB.GetArchetypeList().empty())
			{
				for (auto& Archetype : PPB.GetArchetypeList())
				{
					for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
					{
						auto EntityBB = Archetype->FindComponent<boundingbox>(paperback::vm::PoolDetails{ 0, i });
						auto EntityPos = Archetype->FindComponent<transform>(paperback::vm::PoolDetails{ 0, i });
						auto Entity = Archetype->FindComponent<paperback::component::entity>(paperback::vm::PoolDetails{ 0, i });

						if (!Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID)) //is not a prefab entity
						{
							if (EntityBB) //if the entity has boundingbox
							{
								glm::vec3 CamPos, RayDir;
								float t = 0.0f;

								ImVec2 Min = ImGui::GetWindowContentRegionMin();
								ImVec2 Max = ImGui::GetWindowContentRegionMax();

								Min.x += ImGui::GetWindowPos().x;
								Min.y += ImGui::GetWindowPos().y;
								Max.x += ImGui::GetWindowPos().x;
								Max.y += ImGui::GetWindowPos().y;

								CamPos = PPB.GetSystem< render_system >().m_Camera3D.GetPosition();
								RayDir = PPB.GetViewportMousePosition({ Min.x, Min.y - 70.0f }, { Max.x, Max.y - 70.0f });

								if (RayDir == glm::vec3{})
									continue;


								if (RayAabb({ CamPos.x, CamPos.y, CamPos.z }, { RayDir.x, RayDir.y, RayDir.z }, EntityPos->m_Position + EntityBB->Min, EntityPos->m_Position + EntityBB->Max, t) && m_Imgui.m_bPaused)
								{
									auto& EntityInfo = PPB.GetEntityInfo(Entity->m_GlobalIndex);

									m_Imgui.m_SelectedEntity.first = EntityInfo.m_pArchetype;
									m_Imgui.m_SelectedEntity.second = EntityInfo.m_PoolDetails.m_PoolIndex;
									m_Imgui.m_Components = m_Imgui.m_SelectedEntity.first->GetEntityComponents(m_Imgui.m_SelectedEntity.second);
								}
							}
						}
					}
				}
			}
		}
	}
}

void EditorViewport::Gizmo()
{
	if (m_Imgui.m_SelectedEntity.first)
	{
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();

		float WindowWidth = (float)ImGui::GetWindowWidth();
		float WindowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, WindowWidth, WindowHeight); //setting the viewport

		auto Path = m_Imgui.m_SelectedEntity.first->FindComponent<path>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));
		auto Prefab = m_Imgui.m_SelectedEntity.first->FindComponent<prefab>(paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second }));

		if (!Prefab) //An entity is selected	
			ComposeTransform();

		if (Path && Prefab && m_Imgui.m_SelectedSplinePoint != -1)
		{
			glm::mat4 model{ 1.0f };
			glm::vec3 Trans, Rot, Scale;

			//Camera
			glm::mat4 CameraView = PPB.GetSystem< render_system >().m_Camera3D.GetView();
			glm::mat4 CameraProjection = PPB.GetSystem< render_system >().m_Camera3D.GetProjection();

			model = glm::translate(model, glm::vec3(Path->m_Points.at(m_Imgui.m_SelectedSplinePoint).x, Path->m_Points.at(m_Imgui.m_SelectedSplinePoint).y, Path->m_Points.at(m_Imgui.m_SelectedSplinePoint).z));

			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees

			model = glm::scale(model, glm::vec3(1, 1, 1));//scale = 1,1,1

			ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

			if (ImGuizmo::IsUsing())
			{
				Editor::Math::DecomposeTransform(model, Trans, Scale, Rot);
				Editor::Math::GlmtoVec3(Path->m_Points.at(m_Imgui.m_SelectedSplinePoint), Trans);
			}
		}
	}
}

void EditorViewport::ComposeTransform()
{
	glm::mat4 model{ 1.0f };
	glm::vec3 Trans, Rot, Scale;

	//Camera
	glm::mat4 CameraView = PPB.GetSystem< render_system >().m_Camera3D.GetView();
	glm::mat4 CameraProjection = PPB.GetSystem< render_system >().m_Camera3D.GetProjection();

	auto EntityDetails = paperback::vm::PoolDetails({ 0, m_Imgui.m_SelectedEntity.second });

	auto bTrans = m_Imgui.m_SelectedEntity.first->FindComponent<transform>(EntityDetails);
	auto bRot = m_Imgui.m_SelectedEntity.first->FindComponent<rotation>(EntityDetails);
	auto bScale = m_Imgui.m_SelectedEntity.first->FindComponent<scale>(EntityDetails);
	
	if (bTrans) //for normal entities
		model = glm::translate(model, glm::vec3(bTrans->m_Position.x, bTrans->m_Position.y, bTrans->m_Position.z));

	if (bRot)
	{
		model = glm::rotate(model, glm::radians(bRot->m_Value.x), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(bRot->m_Value.y), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(bRot->m_Value.z), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees
	}
	else
	{
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees
	}

	if (bScale)
		model = glm::scale(model, glm::vec3(bScale->m_Value.x, bScale->m_Value.y, bScale->m_Value.z));

	else
		model = glm::scale(model, glm::vec3(1, 1, 1));//scale = 1,1,1

	ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(model));

	if (ImGuizmo::IsUsing())
	{
		Editor::Math::DecomposeTransform(model, Trans, Scale, Rot);

		if (bTrans)
			Editor::Math::GlmtoVec3(bTrans->m_Position, Trans);

		if (bScale)
			Editor::Math::GlmtoVec3(bScale->m_Value, Scale);
	}
}
