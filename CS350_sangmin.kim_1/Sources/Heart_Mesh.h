///*************************************************************
// *	Author				: Kim sang min.
// *	Assignment number	: Project assignment
// *	Course				: CS250.
// *  Term				: Spring 2020.
// *************************************************************/
//
//#pragma once
//#include "Mesh.h"
//
//class Heart_Mesh : public Mesh
//{
//public:
//	Heart_Mesh();
//	int VertexCount(void) override;
//	Point GetVertex(int i) override;
//	Vector Dimensions(void) override;
//	Point Center(void) override;
//	int FaceCount(void) override;
//	Face GetFace(int i) override;
//	void Set_Normal();
//	//void Add_Vetrtex(Point P, Point Q, Point R, Vector normal);
//
//	/*
//	 * I don't use edge of Heart mesh.
//	 */
//	int EdgeCount(void) override
//	{
//		return 0;
//	}
//	Edge GetEdge(int i) override
//	{
//		(i);
//		return Edge();
//	}
//private:
//	static const Point vertices[16];
//	static const Face faces[28];
//};