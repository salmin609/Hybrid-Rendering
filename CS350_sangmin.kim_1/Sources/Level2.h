#pragma once
#include "Object.h"
#include "State.h"

class LoadedObj;

class Level2 : public State
{
public:
	Level2();
	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;
	~Level2();
private:
	LoadedObj* objMesh;
	Object* obj;
	Material_ emerald;
};
