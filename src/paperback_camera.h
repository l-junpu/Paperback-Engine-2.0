/**********************************************************************************
*\file         paperback_camera.h
*\brief        paperback_camera.h
*
*\author	   Malcolm, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "glm/inc/glm.hpp"

class cam
{
public:
	~cam() = default;

	glm::vec3 GetForwardVector() const;

	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	glm::vec3 GetPosition() const;

	void SetPosition(const glm::vec3& Position);
	void SetTarget(const glm::vec3& Target);

	void MoveForward();
	void MoveBackward();
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();

	void RotateRight(const float m_Speed = 0.1f);
	void RotateLeft(const float m_Speed = 0.1f);
	void RotateUp(const float m_Speed = 0.1f);
	void RotateDown(const float m_Speed = 0.1f);

	void RotateWithMouse(glm::vec2 Direction);

	static cam& GetInstanced();
	cam(const cam&) = delete;
	cam& operator=(const cam&) = delete;

private:
	cam();
	
	void UpdateView();
	void UpdateVectors();

	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::vec3 m_Target;
	glm::vec3 m_Position;

	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_Front;

	float m_Radius;
	float m_Theta;
	float m_Azimuth;
};
