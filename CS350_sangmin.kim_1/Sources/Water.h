#pragma once
#include "Mesh_Object.h"
class Object;

class Water : public Mesh_Object
{
public:
	Water();
    int VertexCount(void) { return 0; }
    Point GetVertex(int i) { (i); return Point{}; }
    Vector Dimensions(void) { return Vector(2, 2, 2); }
    Point Center(void) { return Point(0, 0, 0); }
    int FaceCount(void) { return faceCount; }
    Face GetFace(int i) { (i); return Face{}; }
    int EdgeCount(void) { return 150; }
    Edge GetEdge(int i) { (i); return Edge{}; }
    virtual void SetVertices();
    
private:
    int faceCount = 2;
    //Object* ownerObj;
};
