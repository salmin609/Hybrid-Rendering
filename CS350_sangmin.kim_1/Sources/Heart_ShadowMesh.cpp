/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/
#include "Heart_ShadowMesh.h"

Point HeartShadowMesh::vertices[6] = {
   Point(0.f, 0.f, 0.f),
   Point(0.75f, 0.f, 0.75f),
   Point(0.5f, 0.f, -0.5f),
   Point(0.f, 0.f, -1.f),
   Point(-0.5f, 0.f, -0.5f),
   Point(-0.75f, 0.f, 0.75f),
};

Mesh::Face HeartShadowMesh::faces[4] = {
   Mesh::Face(1,0,2),
   Mesh::Face(2,0,3),
   Mesh::Face(3,0,4),
   Mesh::Face(4,0,5)
};
Mesh::Edge HeartShadowMesh::edge[1] = {
   Mesh::Edge(1,0)
};


HeartShadowMesh::HeartShadowMesh()
{
	Set_Normal();
	//Initialize_Shadow_Mesh();
	Initialize(shader_shadow_vertex.c_str(), shader_shadow_fragment.c_str());
}

void HeartShadowMesh::Set_Normal()
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

//void HeartShadowMesh::Add_Vetrtex(Point P, Point Q, Point R)
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
