#pragma once
#include "Affine.h"
#include "Object.h"

class RaycastManager
{
public:
	RaycastManager();
	Object* RayToObject(Point mousePosWorld) const;
	void MoveCastedObj(Object* selected);
private:
	
	
};
