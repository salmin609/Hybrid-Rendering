// SnubDodecMesh.h
// -- mesh for a snub dodecahedron
// cs250 1/15

#ifndef CS250_SNUBDODEC_H
#define CS250_SNUBDODEC_H

#include "Mesh_Object.h"

class SnubDodecMesh : public Mesh_Object {
  public:
    SnubDodecMesh();
    SnubDodecMesh(std::string vertexPath, std::string fragPath);
    int FaceCount(void) { return 116; }
    virtual void SetVertices();
    std::vector<Vertex> GetTemp();
  private:
    static Face faces[116];
    static Edge edges[150];
    std::vector<Vertex> tempVerticesData;
};


#endif

