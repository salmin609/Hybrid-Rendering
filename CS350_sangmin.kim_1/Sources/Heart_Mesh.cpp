///*************************************************************
// *	Author				: Kim sang min.
// *	Assignment number	: Project assignment
// *	Course				: CS250.
// *  Term				: Spring 2020.
// *************************************************************/
//
//#include "Heart_Mesh.h"
//
//const Point Heart_Mesh::vertices[16] = {
//	Point(0.75f, -0.1f, 1), Point(0.75f, 0.1f, 1),
//	Point(1, -0.1f , 0.5f), Point(1, 0.1f, 0.5f),
//	Point(1, -0.1f, 0), Point(1, 0.1f, 0),
//	Point(0, -0.1f, -1), Point(0, 0.1f, -1),
//	Point(-1, -0.1f, 0),Point(-1, 0.1f, 0),
//	Point(-1, -0.1f, 0.5f), Point(-1, 0.1f, 0.5f),
//	Point(-0.75f, -0.1f, 1),Point(-0.75f, 0.1f, 1),
//	Point(0, -0.1f, 0.25f), Point(0, 0.1f, 0.25f)
//};
//const Heart_Mesh::Face Heart_Mesh::faces[28] = {
//	Face(1,0,2), Face(1,2,3),
//	Face(3,2,4), Face(3,4,5),
//	Face(5,4,6), Face(5,6,7),
//	Face(8,9,6), Face(9,7,6),
//	Face(10,11,8), Face(11,9,8),
//	Face(12,13,11), Face(12,11,10),
//	Face(13,12,14), Face(13,14,15),
//	Face(15,14,0), Face(15,0,1),
//
//	Face(0,14,2),
//	Face(2,14,4),
//	Face(4,14,6),
//	Face(14,8,6),
//	Face(14,10,8),
//	Face(14,12,10),
//
//	Face(15,1,3),
//	Face(15,3,5),
//	Face(15,5,7),
//	Face(15,7,9),
//	Face(15,9,11),
//	Face(15,11,13),
//};
//
//
//Heart_Mesh::Heart_Mesh()
//{
//	Set_Normal();
//	//Initialize_Object_Mesh(shader_object_vertex, shader_object_fragment, true);
//}
//
//int Heart_Mesh::VertexCount()
//{
//	return 16;
//}
//
//Point Heart_Mesh::GetVertex(int i)
//{
//	return vertices[i];
//}
//
//Vector Heart_Mesh::Dimensions()
//{
//	return Vector(2.f, 2.f, 2.f);
//}
//
//Point Heart_Mesh::Center()
//{
//	return Point(0.f, 0.f, 0.f);
//}
//
//int Heart_Mesh::FaceCount()
//{
//	return 28;
//}
//
//Mesh::Face Heart_Mesh::GetFace(int i)
//{
//	return faces[i];
//}
//
//void Heart_Mesh::Set_Normal()
//{
//	const size_t face_count = FaceCount();
//
//	const Point E(0.f, 0.f, 0.f);
//
//	for (size_t i = 0; i < face_count; ++i)
//	{
//		const Mesh::Face face = GetFace(static_cast<int>(i));
//
//		const Point P = vertices[face.index1];
//		const Point Q = vertices[face.index2];
//		const Point R = vertices[face.index3];
//
//		const Vector Q_minus_P = Q - P;
//		const Vector R_minus_P = R - P;
//
//		Vector orientation_vec(cross(Q_minus_P, R_minus_P));
//
//		orientation_vec = normalize(orientation_vec);
//
//		//Add_Vetrtex(P, Q, R, orientation_vec);
//	}
//}
////
////void Heart_Mesh::Add_Vetrtex(Point P, Point Q, Point R, Vector normal)
////{
////	vertex_datas.push_back(P.x);
////	vertex_datas.push_back(P.y);
////	vertex_datas.push_back(P.z);
////
////	vertex_datas.push_back(normal.x);
////	vertex_datas.push_back(normal.y);
////	vertex_datas.push_back(normal.z);
////
////	vertex_datas.push_back(Q.x);
////	vertex_datas.push_back(Q.y);
////	vertex_datas.push_back(Q.z);
////
////	vertex_datas.push_back(normal.x);
////	vertex_datas.push_back(normal.y);
////	vertex_datas.push_back(normal.z);
////
////	vertex_datas.push_back(R.x);
////	vertex_datas.push_back(R.y);
////	vertex_datas.push_back(R.z);
////
////	vertex_datas.push_back(normal.x);
////	vertex_datas.push_back(normal.y);
////	vertex_datas.push_back(normal.z);
////}
