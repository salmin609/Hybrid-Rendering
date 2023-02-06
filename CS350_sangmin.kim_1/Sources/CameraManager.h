#pragma once
#include <SDL2/SDL_events.h>
#include "Camera.h"

class CameraManager
{
public:
	static CameraManager* instance;
	CameraManager();
	void CameraMovement(SDL_Event event);
	void SetMoveFaster(bool toggle);
	void SetTraverseMode(bool toggle);
	void SetVV();
	Camera* GetCamera();
	Point CameraPos();
	void UpdateAB(float newAlpha, float newBeta);
	void Init();
	void Move(SDL_Keycode keycode);
	void SetCameraPos(Vector3 cameraPos, Vector3 cameraLookAt);
	void CameraReInit();
	Vector GetLookAtDirection();
	void Switch(int i);
private:
	Vector ey = Vector(0, 1, 0);
	float pi = 4.0f * atan(1.0f);
	//Sphereical cam
	Point defaultCamPos{ 0.f, 1.f, 2.f };
	Vector v_v;
	bool traverse_mode;
	Vector lookat;
	Camera cam;
	
	float cam_alpha;
	float cam_beta;
	float r;
	bool move_faster = false;
	float aspect = 1.f;
};
