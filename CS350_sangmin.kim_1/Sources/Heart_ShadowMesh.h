/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "Mesh.h"

class HeartShadowMesh : public Mesh
{
public:
    HeartShadowMesh();
    int VertexCount() { return 6; }
    Point GetVertex(int i) { return vertices[i]; }
    Vector Dimensions() { return Vector(2, 0, 0); }
    Point Center() { return Point(0, 0, 0); }
    int FaceCount() { return 4; }
    Face GetFace(int i) { return faces[i]; }
    int EdgeCount() { return 0; }
    Edge GetEdge(int i) { return edge[i]; }
    void Set_Normal();
    //void Add_Vetrtex(Point P, Point Q, Point R);

private:
    static Point vertices[6];
    static Face faces[4];
    static Edge edge[1];
};
