/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#ifndef CS250_SimpleCubeMesh_H
#define CS250_SimpleCubeMesh_H

#include "Mesh.h"

class SimpleCubeMesh : public Mesh {
public:
    SimpleCubeMesh(bool is_light = false);
    int FaceCount(void) override;

private:
    static const Point vertices[8];
    static const Face faces[12];
    static const Edge edges[12];
};

#endif

