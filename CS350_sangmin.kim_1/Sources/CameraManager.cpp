#include "CameraManager.h"
#include <iostream>
#include "Graphic.h"

CameraManager* CameraManager::instance = nullptr;

CameraManager::CameraManager()
{
	if(instance != nullptr)
	{
		std::cout << "There are two cameraManager delete one" << std::endl;
		delete instance;
	}
	instance = this;
	Init();

	cam = Camera(defaultCamPos, Vector{ 0.f, 0.f, -1.f }, ey, 0.5f * pi, 1.f, 0.01f, 1000.f);
}

void CameraManager::Init()
{
	cam_alpha = 0.f;
	cam_beta = pi;
	r = 1.f;
	SetVV();
}

void CameraManager::Move(SDL_Keycode keycode)
{
	float speed = 0.1f;

	if (move_faster)
	{
		speed = 0.5f;
	}

	Vector cameraRight = cam.Right() * speed;
	switch (keycode)
	{
	case SDLK_w:
		defaultCamPos.x += (v_v.x * speed);
		defaultCamPos.y += (v_v.y * speed);
		defaultCamPos.z += (v_v.z * speed);
		break;
	case SDLK_a:
		defaultCamPos.x -= cameraRight.x;
		defaultCamPos.y -= cameraRight.y;
		defaultCamPos.z -= cameraRight.z;
		break;
	case SDLK_s:
		defaultCamPos.x -= (v_v.x * speed);
		defaultCamPos.y -= (v_v.y * speed);
		defaultCamPos.z -= (v_v.z * speed);
		break;
	case SDLK_d:
		defaultCamPos.x += cameraRight.x;
		defaultCamPos.y += cameraRight.y;
		defaultCamPos.z += cameraRight.z;
		break;
	}

	CameraReInit();
}

void CameraManager::SetCameraPos(Vector3 cameraPos, Vector3 cameraLookAt)
{
	defaultCamPos.x = cameraPos.x;
	defaultCamPos.y = cameraPos.y;
	defaultCamPos.z = cameraPos.z;

	lookat.x = cameraLookAt.x;
	lookat.y = cameraLookAt.y;
	lookat.z = cameraLookAt.z;
	CameraReInit();
}

void CameraManager::CameraReInit()
{
	cam = Camera(defaultCamPos, lookat, ey, 0.5f * pi, 1.f, 0.01f, 1000.f);
}

Vector CameraManager::GetLookAtDirection()
{
	return lookat;
}

void CameraManager::Switch(int i)
{
	switch(i)
	{
	case 0:
		cam = Camera(defaultCamPos, Vector{1.f, 0.f, 0.f}, ey, 0.5f * pi, 1.f, 0.01f, 1000.f);
		break;
	case 1:
		cam = Camera(defaultCamPos, Vector{ -1.f, 0.f, 0.f }, ey, 0.5f * pi, 1.f, 0.01f, 1000.f);
		break;
	case 2:
		cam = Camera(defaultCamPos, Vector{ 0.f, 1.f, 0.f }, Vector{0.f, 0.f, -1.f}, 0.5f * pi, 1.f, 0.01f, 1000.f);
		break;
	case 3:
		cam = Camera(defaultCamPos, Vector{ 0.f, -1.f, 0.f }, Vector{0.f, 0.f, 1.f}, 0.5f * pi, 1.f, 0.01f, 1000.f);
		break;
	case 4:
		cam = Camera(defaultCamPos, Vector{ 0.f, 0.f, 1.f }, ey, 0.5f * pi, 1.f, 0.01f, 1000.f);
		break;
	case 5:
		cam = Camera(defaultCamPos, Vector{ 0.f, 0.f, -1.f }, ey, 0.5f * pi, 1.f, 0.01f, 1000.f);
		break;

	}
}

void CameraManager::CameraMovement(SDL_Event event)
{
	float speed = 0.1f;

	if (move_faster)
	{
		speed = 0.5f;
	}

	const bool is_mouse_wheel_up = event.wheel.y > 0;

	if (traverse_mode)
	{
		if (is_mouse_wheel_up)
		{
			defaultCamPos.x += (v_v.x * speed);
			defaultCamPos.y += (v_v.y * speed);
			defaultCamPos.z += (v_v.z * speed);
		}
		CameraReInit();
	}
	else
	{
		if (is_mouse_wheel_up)
		{
			if (Graphic::instance->SelectedObject() != nullptr)
			{
				Point& obj_pos = Graphic::instance->SelectedObject()->Get_Obj_Pos();
				obj_pos.z += (0.1f);
			}
		}
	}

	if (traverse_mode == true)
	{
		if (!is_mouse_wheel_up)
		{
			defaultCamPos.x -= (v_v.x * speed);
			defaultCamPos.y -= (v_v.y * speed);
			defaultCamPos.z -= (v_v.z * speed);
		}
		CameraReInit();
	}
	else
	{
		if (!is_mouse_wheel_up)
		{
			if (Graphic::instance->SelectedObject() != nullptr)
			{
				Point& obj_pos = Graphic::instance->SelectedObject()->Get_Obj_Pos();
				obj_pos.z -= (0.1f);
			}
		}
	}
}

void CameraManager::SetMoveFaster(bool toggle)
{
	move_faster = toggle;
}

void CameraManager::SetTraverseMode(bool toggle)
{
	traverse_mode = toggle;
}

void CameraManager::SetVV()
{
	v_v.x = r * cos(cam_alpha) * sin(cam_beta);
	v_v.y = r * sin(cam_alpha);
	v_v.z = r * cos(cam_alpha) * cos(cam_beta);
	lookat = v_v;
	CameraReInit();
}

Camera* CameraManager::GetCamera()
{
	return &cam;
}

Point CameraManager::CameraPos()
{
	return defaultCamPos;
}

void CameraManager::UpdateAB(float newAlpha, float newBeta)
{
	cam_alpha += newAlpha;
	cam_beta += newBeta;

	if (cam_alpha < -(pi / 2))
	{
		cam_alpha = -(pi / (2.0001f));
	}
	if (cam_alpha > (pi / 2))
	{
		cam_alpha = (pi / 2);
	}
}


