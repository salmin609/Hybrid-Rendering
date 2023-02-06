#include "Level3.h"

#include <iostream>


#include "Graphic.h"
#include "Particle.h"

Level3::Level3()
{
	particle = new Particle();
	object = new Object(particle, Point{ 0.f, 0.f, 0.f }, nullptr);
}

void Level3::Load()
{
	Graphic::objects.clear();
	Graphic::objects.push_back(object);
	CameraManager::instance->SetCameraPos(Vector3{ -7.f, 1.f, -5.f }, Vector3{0.8f, -0.09f, 0.45f});
	//Graphic::objects.push_back(Graphic::light);
	Graphic::groundId = nullptr;
	Graphic::water = nullptr;
}

void Level3::Update(float dt)
{
	particle->MoveParticle(dt);
	/*Vector camPos = CameraManager::instance->GetLookAtDirection();

	std::cout << "  " << camPos.x << "  " << camPos.y << "  " << camPos.z << std::endl;*/
}

void Level3::UnLoad()
{
}

Level3::~Level3()
{
	delete particle;
	delete object;
}
