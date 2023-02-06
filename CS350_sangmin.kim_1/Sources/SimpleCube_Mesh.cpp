/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "SimpleCube_Mesh.h"


const Point SimpleCubeMesh::vertices[8] = {
	Point(1,-1,1), //0
	Point(1,1,1), //1
	Point(1,-1, -1), //2
	Point(1,1,-1), //3
	Point(-1,-1,1), //4
	Point(-1,1,1), //5
	Point(-1,-1,-1), //6
	Point(-1,1,-1) //7
};
const SimpleCubeMesh::Face SimpleCubeMesh::faces[12] = {
	Face(0,4,6), Face(0,6,2),
	Face(1,3,7), Face(1,7,5),
	Face(4,5,7), Face(4,7,6),
	Face(0,2,3), Face(0,3,1),
	Face(0,1,4), Face(1,5,4),
	Face(2,6,3), Face(3,6,7)
};
const SimpleCubeMesh::Edge SimpleCubeMesh::edges[12] = {
	Edge(0,4), Edge(4, 6),
	Edge(6,2), Edge(2,0),
	Edge(4,5), Edge(5,7),
	Edge(7,6), Edge(2,3),
	Edge(3,1), Edge(1,0),
	Edge(1,5), Edge(7,3)
};

SimpleCubeMesh::SimpleCubeMesh(bool is_light)
{
	if(!is_light)
	{
		//Set_Normal();
		//Initialize_Ground_Mesh();
		Initialize(shader_simple_vertex.c_str(), shader_simple_fragment.c_str());
	}
	else
	{
		//Set_Normal();
		Initialize(shader_light_vertex.c_str(), shader_light_fragment.c_str());
		//Initialize_Ground_Mesh(shader_light_vertex, shader_light_fragment);
	}
}

int SimpleCubeMesh::FaceCount()
{
	/*
	*	Since professor mentioned "as simple as possible", and
	*	also the faces is const static array, return just size of faces using magic number.
	*/
	return 12;
}

