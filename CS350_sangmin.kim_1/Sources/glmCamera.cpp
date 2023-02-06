#include <glm/gtc/matrix_transform.hpp>
#include "glmCamera.h"

GLMCamera::GLMCamera(glm::vec3 position/*= glm::vec3(0.0f, 0.0f, 0.0f)*/, glm::vec3 up/*= glm::vec3(0.0f, 1.0f, 0.0f)*/, float yaw/*= YAW*/, float pitch/*= PITCH*/)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

GLMCamera::GLMCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 GLMCamera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void GLMCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == Camera_Movement::FORWARD)
		Position += Front * velocity;
	if (direction == Camera_Movement::BACKWARD)
		Position -= Front * velocity;
	if (direction == Camera_Movement::LEFT)
		Position -= Right * velocity;
	if (direction == Camera_Movement::RIGHT)
		Position += Right * velocity;
}
void GLMCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch/* = true*/)
{
	float x = xoffset * MouseSensitivity;
	float y = yoffset * MouseSensitivity;

	Yaw += x;
	Pitch += y;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	updateCameraVectors();
}

void GLMCamera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 60.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 60.0f)
		Zoom = 60.0f;
}

void GLMCamera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}