/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "Mesh.h"

class SquareMesh : public Mesh
{
public:
    SquareMesh();
    int VertexCount() { return 4; }
    Point GetVertex(int i) { return vertices[i]; }
    Vector Dimensions() { return Vector(2, 0, 0); }
    Point Center() { return Point(0, 0, 0); }
    int FaceCount() { return 2; }
    Face GetFace(int i) { return faces[i]; }
    int EdgeCount() { return 4; }
    Edge GetEdge(int i) { return edges[i]; }
    void Set_Normal();
    //void Add_Vetrtex(Point P, Point Q, Point R);

private:
    static Point vertices[4];
    static Face faces[2];
    static Edge edges[4];
};
