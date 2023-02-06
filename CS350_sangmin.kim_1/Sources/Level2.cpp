#include "Level2.h"
#include "Graphic.h"
#include "LoadedObj.h"

Level2::Level2()
{
	emerald.ambient = Vector(0.0215f, 0.1745f, 0.0215f);
	emerald.diffuse = Vector(0.07568f, 0.61424f, 0.07568f);
	emerald.specular = Vector(0.633f, 0.727811f, 0.633f);
	emerald.shiness = 0.6f;
	const Vector scalingVec{ 4.f, 4.f, 4.f };
	objMesh = new LoadedObj("Dog", true, true);
	obj = new Object(objMesh, Point(0.f, 0.f, 0.f), &emerald);
	obj->Set_Scale(scalingVec);
}

void Level2::Load()
{
	Graphic::objects.clear();
	Graphic::objects.push_back(obj);
	CameraManager::instance->SetCameraPos(Vector3{ 0.f, 0.f, 0.f}, Vector3{ 0.f, 0.f, 1.f});
	//Graphic::objects.push_back(Graphic::light);
	Graphic::groundId = nullptr;
	Graphic::water = nullptr;
}

void Level2::Update(float dt)
{
	(dt);
}

void Level2::UnLoad()
{
	
}

Level2::~Level2()
{
	delete objMesh;
	delete obj;
}
