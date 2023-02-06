#pragma once
#include "Mesh_Object.h"
class Particle : public Mesh_Object
{
public:
    Particle();
    int FaceCount(void) { return faceCount; }
    virtual void SetVertices();
    void MoveParticle(float dt);
private:
    int faceCount = 12;
	
};