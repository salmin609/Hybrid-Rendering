/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#ifndef CS250_CUBEMESH_H
#define CS250_CUBEMESH_H

#include "Mesh_Object.h"

class CubeMesh : public Mesh_Object {
  public:
    CubeMesh();
    int FaceCount(void) override;
    virtual void SetVertices();

  private:
    static const Face faces[12];
    static const Edge edges[12];
};

#endif

