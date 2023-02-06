/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "Square_Mesh.h"


Point SquareMesh::vertices[4] = {
   Point(-1.f, 0.f, -1.f),
   Point(-1.f, 0.f, 1.f),
   Point(1.f, 0.f, 1.f),
   Point(1.f, 0.f, -1.f)
};

Mesh::Face SquareMesh::faces[2] = {
   Mesh::Face(0, 1, 2), 
   Mesh::Face(2, 3, 0)
};
Mesh::Edge SquareMesh::edges[4] = {
   Mesh::Edge(0, 1),
   Mesh::Edge(1, 2),
   Mesh::Edge(2, 3),
   Mesh::Edge(1, 3)
};

SquareMesh::SquareMesh()
{
	Set_Normal();
	Initialize(shader_shadow_vertex.c_str(), shader_shadow_fragment.c_str());
}

void SquareMesh::Set_Normal()
{
	const size_t face_count = FaceCount();

	const Point E(0.f, 0.f, 0.f);

	for (size_t i = 0; i < face_count; ++i)
	{
		const Mesh::Face face = GetFace(static_cast<int>(i));

		const Point P = vertices[face.index1];
		const Point Q = vertices[face.index2];
		const Point R = vertices[face.index3];

		const Vector Q_minus_P = Q - P;
		const Vector R_minus_P = R - P;

		Vector orientation_vec(cross(Q_minus_P, R_minus_P));

		orientation_vec = normalize(orientation_vec);

		//Add_Vetrtex(P, Q, R);
	}
}

//void SquareMesh::Add_Vetrtex(Point P, Point Q, Point R)
//{
//	vertex_datas.push_back(P.x);
//	vertex_datas.push_back(P.y);
//	vertex_datas.push_back(P.z);
//
//	vertex_datas.push_back(Q.x);
//	vertex_datas.push_back(Q.y);
//	vertex_datas.push_back(Q.z);
//
//	vertex_datas.push_back(R.x);
//	vertex_datas.push_back(R.y);
//	vertex_datas.push_back(R.z);
//}
