#include "Level4.h"

#include "Graphic.h"
#include "Ground.h"
#include "Object.h"
#include "Water.h"

Level4::Level4()
{
	emeraldMat.ambient = Vector(0.0215f, 0.1745f, 0.0215f);
	emeraldMat.diffuse = Vector(0.07568f, 0.61424f, 0.07568f);
	emeraldMat.specular = Vector(0.633f, 0.727811f, 0.633f);
	emeraldMat.shiness = 0.6f;

	obcidianMat.ambient = Vector(0.05375f, 0.05f, 0.06625f);
	obcidianMat.diffuse = Vector(0.18275f, 0.17f, 0.22525f);
	obcidianMat.specular = Vector(0.332741f, 0.328634f, 0.346435f);
	obcidianMat.shiness = 0.3f;

	water = new Water();
	ground = new Object(&cube_mesh, Point(0.f, -3.f, 0.f), nullptr);
	downCube = new Object(&cube_mesh, Point(0.f, -3.f, 0.f), &obcidianMat);

	upFirstSnub = new Object(&snub_mesh, Point(1.f, 2.f, 0.f), &emeraldMat);
	upSecondSnub = new Object(&snub_mesh, Point(-1.f, 2.f, 0.f), &emeraldMat);

	downFirstSnub = new Object(&snub_mesh, Point(1.f, -2.f, 0.f), &obcidianMat);
	downSecondSnub = new Object(&snub_mesh, Point(-1.f, -2.f, 0.f), &obcidianMat);

	//waterObj = new Object(water, Point{ 0.f, 1.f, 0.f }, nullptr);
	downCube->Set_Scale(Vector(10.f, -0.1f, 10.f));
	ground->Set_Scale(Vector(10.f, -0.1f, 10.f));
	//waterObj->Set_Scale(Vector(10.f, -0.1f, 10.f));
	upFirstSnub->Set_Scale(0.35f);
	upSecondSnub->Set_Scale(0.35f);
	downFirstSnub->Set_Scale(0.35f);
	downSecondSnub->Set_Scale(0.35f);
	
}

void Level4::Load()
{
	Graphic::objects.clear();
	Graphic::objects.push_back(upFirstSnub);
	Graphic::objects.push_back(upSecondSnub);
	Graphic::objects.push_back(downFirstSnub);
	Graphic::objects.push_back(downSecondSnub);
	//Graphic::objects.push_back(downCube);
	Graphic::objects.push_back(Graphic::light);
	CameraManager::instance->SetCameraPos(Vector3{ 0.f, 5.f, -5.f }, Vector3{ 0.f, -1.f, 1.f });
	Graphic::groundId = Ground::groundShader;
	Graphic::water = waterObj;
}

void Level4::Update(float dt)
{
	(dt);
}

void Level4::UnLoad()
{
	
}

Level4::~Level4()
{
	delete waterObj;
	delete ground;
	delete upFirstSnub;
	delete upSecondSnub;
	delete downFirstSnub;
	delete downSecondSnub;
	delete downCube;
	delete water;
}
