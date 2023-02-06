/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "CubeMesh.h"

const CubeMesh::Face CubeMesh::faces[12] = {
	Face(0,4,6), Face(0,6,2),
	Face(1,3,7), Face(1,7,5),
	Face(4,5,7), Face(4,7,6),
	Face(0,2,3), Face(0,3,1),
	Face(0,1,4), Face(1,5,4),
	Face(2,6,3), Face(3,6,7)
};
const CubeMesh::Edge CubeMesh::edges[12] = {
	Edge(0,4), Edge(4, 6),
	Edge(6,2), Edge(2,0),
	Edge(4,5), Edge(5,7),
	Edge(7,6), Edge(2,3),
	Edge(3,1), Edge(1,0),
	Edge(1,5), Edge(7,3)
};

CubeMesh::CubeMesh()
{
	CubeMesh::SetVertices();
	Initialize(shader_object_vertex.c_str(), shader_object_fragment.c_str());
}


int CubeMesh::FaceCount()
{
	/*
	*	Since professor mentioned "as simple as possible", and
	*	also the faces is const static array, return just size of faces using magic number.
	*/
	return 12;
}

void CubeMesh::SetVertices()
{
	const int vert_count = 8;

	const Point vertarr[] = {
		Point(1, -1, 1),
		Point(1, 1, 1),
		Point(1, -1, -1),
		Point(1, 1, -1),
		Point(-1, -1, 1),
		Point(-1, 1, 1),
		Point(-1, -1, -1),
		Point(-1, 1, -1)
	};

	vertices.insert(vertices.end(), vertarr, vertarr + vert_count);

	for (int i = 0; i < vert_count; ++i)
	{
		normal.push_back(Hcoord());
	}

	const int faceSize = FaceCount();

	for (int i = 0; i < faceSize; ++i)
	{
		Face face = faces[i];
		const int index1 = face.index1;
		const int index2 = face.index2;
		const int index3 = face.index3;

		elements.push_back(index1);
		elements.push_back(index2);
		elements.push_back(index3);
	}

	for (int i = 0; i < faceSize; ++i)
	{
		Face face = faces[i];
		const int index1 = face.index1;
		const int index2 = face.index2;
		const int index3 = face.index3;

		Point P = vertices[index1];
		Point Q = vertices[index2];
		Point R = vertices[index3];

		const Vector Q_minus_P = Q - P;
		const Vector R_minus_P = R - P;

		Vector orientation_vec(cross(Q_minus_P, R_minus_P));

		orientation_vec = normalize(orientation_vec);

		normal[index1] += orientation_vec;
		normal[index2] += orientation_vec;
		normal[index3] += orientation_vec;
	}
	InitializeVerticesDatas();
}