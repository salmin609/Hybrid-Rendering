#pragma once
#include "CubeMesh.h"
#include "Object.h"
#include "SnubDodecMesh.h"
#include "State.h"

class Water;
class Object;

class Level4 : public State
{
public:
	Level4();
	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;
	~Level4();

private:
	Object* waterObj;
	Object* ground;
	Object* upFirstSnub;
	Object* upSecondSnub;
	Object* downFirstSnub;
	Object* downSecondSnub;
	Object* downCube;
	Water* water;
	SnubDodecMesh snub_mesh;
	CubeMesh cube_mesh;
	Material_ emeraldMat;
	Material_ obcidianMat;
};
