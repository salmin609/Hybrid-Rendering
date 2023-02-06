#include "State.h"
#include "SnubDodecMesh.h"
#include "CubeMesh.h"
#include "Light_Mesh.h"
#include "LoadedObj.h"
#include "Object.h"

class Ground;

class Level1 : public State
{
public:
	Level1();
	virtual void Load();
	virtual void Update(float dt);
	virtual void UnLoad();
	~Level1();
	void Init_Objects();
	void Initialize_Material();
private:
	SnubDodecMesh snub_mesh;
	CubeMesh cube_mesh;
	Light_Mesh light_mesh;
	LoadedObj* deerObj;
	LoadedObj* quadObj;
	
	Object* center_circle;
	Object* right_circle;
	Object* light;
	
	Object* deer;
	Object* tree;

	Material_ emerald;
	Material_ obsidian;
	Material_ jade;
	Material_ pearl;
	Material_ ruby;
	Material_ chrome;
	Material_ red_plastic;

	Ground* ground;
};
