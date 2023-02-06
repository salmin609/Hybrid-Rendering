#pragma once

#include "Mesh_Object.h"

class LoadedObj : public Mesh_Object
{
public :
    LoadedObj();
    LoadedObj(std::string filePath, bool isTextured = false, bool instacing = false);
    int FaceCount(void) { return faceCount; }
    virtual void SetVertices();
    void SetIsRoughModel(bool trigger);
    ~LoadedObj();
private :
    std::string file;
    bool consistNormal = false;
    bool consistTexture = false;
    int faceCount = 0;
};
