/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#ifndef CS250_SPHEREMESH_H
#define CS250_SPHEREMESH_H

#include "Mesh.h"

class SphereMesh : public Mesh {
public:
    SphereMesh();
    int FaceCount(void) override;

private:
    static const Point vertices[9];
    static const Face faces[8];
    static const Edge edges[12];
};

#endif

