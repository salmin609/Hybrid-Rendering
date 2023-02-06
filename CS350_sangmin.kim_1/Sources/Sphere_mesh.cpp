/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/
#include "Sphere_mesh.h"

const Point SphereMesh::vertices[9] = {
	Point(0,0,0),//0
	Point(0,0,1),//1
	Point(0.75f,0,0.75f),//2
	Point(1,0,0),//3
	Point(0.75f,0,-0.75f),//4
	Point(0,0,-1.f),//5
	Point(-0.75f,0,-0.75f),//6
	Point(-1.f,0,0),//7
	Point(-0.75f,0,0.75f),//8
};
const SphereMesh::Face SphereMesh::faces[8] = {
	Face(1,0,2), 
	Face(2,0,3),
	Face(3,0,4), 
	Face(4,0,5),
	Face(5,0,6), 
	Face(6,0,7),
	Face(7,0,8), 
	Face(8,0,1),
};
const SphereMesh::Edge SphereMesh::edges[12] = {
	Edge(0,4), Edge(4, 6),
	Edge(6,2), Edge(2,0),
	Edge(4,5), Edge(5,7),
	Edge(7,6), Edge(2,3),
	Edge(3,1), Edge(1,0),
	Edge(1,5), Edge(7,3)
};

SphereMesh::SphereMesh()
{
	//Set_Normal();
	//Initialize_Shadow_Mesh();
	Initialize(shader_shadow_vertex.c_str(), shader_shadow_fragment.c_str());
}

int SphereMesh::FaceCount()
{
	/*
	*	Since professor mentioned "as simple as possible", and
	*	also the faces is const static array, return just size of faces using magic number.
	*/
	return 8;
}

