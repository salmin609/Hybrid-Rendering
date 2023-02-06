/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/
#include "SnubDodecMesh.h"


Mesh::Face SnubDodecMesh::faces[116] = {
  Face(48,32,45), Face(48,16,32), Face(48,0,16),  Face(54,20,47),
  Face(54,1,20),  Face(54,24,1),  Face(36,56,30), Face(36,22,56),
  Face(36,2,22),  Face(37,23,57), Face(37,29,23), Face(37,3,29),
  Face(49,41,21), Face(49,31,41), Face(49,4,31),  Face(39,35,58),
  Face(39,5,35),  Face(39,12,5),  Face(50,17,33), Face(50,40,17),
  Face(50,6,40),  Face(51,44,34), Face(51,7,44),  Face(51,13,7),
  Face(52,8,46),  Face(52,25,8),  Face(52,15,25), Face(53,26,9),
  Face(53,42,26), Face(53,19,42), Face(55,38,10), Face(55,18,38),
  Face(55,28,18), Face(27,59,43), Face(27,11,59), Face(27,14,11),
  Face(0,10,7),   Face(4,11,1),   Face(2,8,5),    Face(6,9,3),
  Face(12,19,22), Face(13,23,16), Face(14,17,20), Face(15,21,18),
  Face(24,34,31), Face(28,35,25), Face(29,26,32), Face(27,30,33),
  Face(43,46,36), Face(37,47,40), Face(41,44,38), Face(39,45,42),
  Face(48,58,55), Face(52,59,49), Face(53,50,56), Face(51,54,57),
  Face(0,7,16),   Face(1,11,20),  Face(2,5,22),   Face(3,9,29),
  Face(4,1,31),   Face(5,8,35),   Face(6,3,40),   Face(7,10,44),
  Face(8,2,46),   Face(9,6,53),   Face(10,0,55),  Face(11,4,59),
  Face(12,22,5),  Face(13,16,7),  Face(14,20,11), Face(15,18,25),
  Face(16,23,32), Face(17,14,33), Face(18,21,38), Face(19,12,42),
  Face(20,17,47), Face(21,15,49), Face(22,19,56), Face(23,13,57),
  Face(24,31,1),  Face(25,35,8),  Face(26,29,9),  Face(27,33,14),
  Face(28,25,18), Face(29,32,23), Face(30,27,36), Face(31,34,41),
  Face(32,26,45), Face(33,30,50), Face(34,24,51), Face(35,28,58),
  Face(36,46,2),  Face(37,40,3),  Face(38,44,10), Face(39,42,12),
  Face(40,47,17), Face(41,38,21), Face(42,45,26), Face(43,36,27),
  Face(44,41,34), Face(45,39,48), Face(46,43,52), Face(47,37,54),
  Face(48,55,0),  Face(49,59,4),  Face(50,53,6),  Face(51,57,13),
  Face(52,49,15), Face(53,56,19), Face(54,51,24), Face(55,58,28),
  Face(56,50,30), Face(57,54,37), Face(58,48,39), Face(59,52,43)
};


Mesh::Edge SnubDodecMesh::edges[150] = {
  Edge(0,16),  Edge(1,20),  Edge(2,22),  Edge(3,29),  Edge(4,31),
  Edge(5,35),  Edge(6,40),  Edge(7,44),  Edge(8,46),  Edge(9,53),
  Edge(10,55), Edge(11,59), Edge(12,5),  Edge(13,7),  Edge(14,11),
  Edge(15,25), Edge(16,32), Edge(17,33), Edge(18,38), Edge(19,42),
  Edge(20,47), Edge(21,49), Edge(22,56), Edge(23,57), Edge(24,1),
  Edge(25,8),  Edge(26,9),  Edge(27,14), Edge(28,18), Edge(29,23),
  Edge(30,36), Edge(31,41), Edge(32,45), Edge(33,50), Edge(34,51),
  Edge(35,58), Edge(36,2),  Edge(37,3),  Edge(38,10), Edge(39,12),
  Edge(40,17), Edge(41,21), Edge(42,26), Edge(43,27), Edge(44,34),
  Edge(45,48), Edge(46,52), Edge(47,54), Edge(48,0),  Edge(49,4),
  Edge(50,6),  Edge(51,13), Edge(52,15), Edge(53,19), Edge(54,24),
  Edge(55,28), Edge(56,30), Edge(57,37), Edge(58,39), Edge(59,43),
  Edge(0,7),   Edge(1,11),  Edge(2,5),   Edge(3,9),   Edge(4,1),
  Edge(5,8),   Edge(6,3),   Edge(7,10),  Edge(8,2),   Edge(9,6),
  Edge(10,0),  Edge(11,4),  Edge(12,22), Edge(13,16), Edge(14,20),
  Edge(15,18), Edge(16,23), Edge(17,14), Edge(18,21), Edge(19,12),
  Edge(20,17), Edge(21,15), Edge(22,19), Edge(23,13), Edge(24,31),
  Edge(25,35), Edge(26,29), Edge(27,33), Edge(28,25), Edge(29,32),
  Edge(30,27), Edge(31,34), Edge(32,26), Edge(33,30), Edge(34,24),
  Edge(35,28), Edge(36,46), Edge(37,40), Edge(38,44), Edge(39,42),
  Edge(40,47), Edge(41,38), Edge(42,45), Edge(43,36), Edge(44,41),
  Edge(45,39), Edge(46,43), Edge(47,37), Edge(48,55), Edge(49,59),
  Edge(50,53), Edge(51,57), Edge(52,49), Edge(53,56), Edge(54,51),
  Edge(55,58), Edge(56,50), Edge(57,54), Edge(58,48), Edge(59,52),
  Edge(0,55),  Edge(1,31),  Edge(2,46),  Edge(3,40),  Edge(4,59),
  Edge(5,22),  Edge(6,53),  Edge(7,16),  Edge(8,35),  Edge(9,29),
  Edge(10,44), Edge(11,20), Edge(12,42), Edge(13,57), Edge(14,33),
  Edge(15,49), Edge(17,47), Edge(25,18), Edge(19,56), Edge(21,38),
  Edge(23,32), Edge(24,51), Edge(26,45), Edge(27,36), Edge(28,58),
  Edge(30,50), Edge(41,34), Edge(37,54), Edge(39,48), Edge(52,43)
};

SnubDodecMesh::SnubDodecMesh()
{
    SnubDodecMesh::SetVertices();
    Initialize(shader_object_vertex.c_str(), shader_object_fragment.c_str());
}

SnubDodecMesh::SnubDodecMesh(std::string vertexPath, std::string fragPath)
{
    SnubDodecMesh::SetVertices();
    Initialize(vertexPath.c_str(), fragPath.c_str());
}

void SnubDodecMesh::SetVertices()
{
    const Point vertarr[] = {
    	Point(0.8734f, 0.2507f, 0.2840f), Point(-0.1461f, -0.9260f, -0.1655f),
        Point(-0.7273f, 0.4871f, 0.3743f), Point(0.1655f, 0.1461f, -0.9260f),
        Point(-0.2840f, -0.8734f, 0.2507f), Point(-0.3743f, 0.7273f, 0.4871f),
        Point(-0.2507f, 0.2840f, -0.8734f), Point(0.9260f, -0.1655f, 0.1461f),
        Point(-0.4871f, 0.3743f, 0.7273f), Point(0.0852f, 0.5518f, -0.7711f),
        Point(0.7711f, -0.0852f, 0.5518f), Point(-0.5518f, -0.7711f, -0.0852f),
        Point(-0.1461f, 0.9260f, 0.1655f), Point(0.8734f, -0.2507f, -0.2840f),
        Point(-0.7273f, -0.4871f, -0.3743f), Point(-0.2507f, -0.2840f, 0.8734f),
        Point(0.9260f, 0.1655f, -0.1461f), Point(-0.4871f, -0.3743f, -0.7273f),
        Point(0.1655f, -0.1461f, 0.9260f), Point(-0.2840f, 0.8734f, -0.2507f),
        Point(-0.3743f, -0.7273f, -0.4871f), Point(0.0852f, -0.5518f, 0.7711f),
        Point(-0.5518f, 0.7711f, 0.0852f), Point(0.7711f, 0.0852f, -0.5518f),
        Point(0.2840f, -0.8734f, -0.2507f), Point(-0.1655f, 0.1461f, 0.9260f),
        Point(0.3743f, 0.7273f, -0.4871f), Point(-0.9260f, -0.1655f, -0.1461f),
        Point(0.2507f, 0.2840f, 0.8734f), Point(0.4871f, 0.3743f, -0.7273f),
        Point(-0.8734f, 0.2507f, -0.2840f), Point(0.1461f, -0.9260f, 0.1655f),
        Point(0.7273f, 0.4871f, -0.3743f), Point(-0.7711f, -0.0852f, -0.5518f),
        Point(0.5518f, -0.7711f, 0.0852f), Point(-0.0852f, 0.5518f, 0.7711f),
        Point(-0.9260f, 0.1655f, 0.1461f), Point(0.2507f, -0.2840f, -0.8734f),
        Point(0.4871f, -0.3743f, 0.7273f), Point(0.2840f, 0.8734f, 0.2507f),
        Point(-0.1655f, -0.1461f, -0.9260f), Point(0.3743f, -0.7273f, 0.4871f),
        Point(0.1461f, 0.9260f, -0.1655f), Point(-0.8734f, -0.2507f, 0.2840f),
        Point(0.7273f, -0.4871f, 0.3743f), Point(0.5518f, 0.7711f, -0.0852f),
        Point(-0.7711f, 0.0852f, 0.5518f), Point(-0.0852f, -0.5518f, -0.7711f),
        Point(0.6421f, 0.6250f, 0.3216f), Point(-0.3216f, -0.6421f, 0.6250f),
        Point(-0.6250f, 0.3216f, -0.6421f), Point(0.6421f, -0.6250f, -0.3216f),
        Point(-0.6250f, -0.3216f, 0.6421f), Point(-0.3216f, 0.6421f, -0.6250f),
        Point(0.3216f, -0.6421f, -0.6250f), Point(0.6250f, 0.3216f, 0.6421f),
        Point(-0.6421f, 0.6250f, -0.3216f), Point(0.6250f, -0.3216f, -0.6421f),
        Point(0.3216f, 0.6421f, 0.6250f), Point(-0.6421f, -0.6250f, 0.3216f) };

    vertices.insert(vertices.end(), vertarr, vertarr + 60);

    for (int i = 0; i < 60; ++i)
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


    for (int i = 0; i < faceSize; ++i)
    {
        Face face = faces[i];
        const int index1 = face.index1;
        const int index2 = face.index2;
        const int index3 = face.index3;

        Point point1 = vertarr[index1];
        Point point2 = vertarr[index2];
        Point point3 = vertarr[index3];

        Hcoord normal1 = normal[index1];
        Hcoord normal2 = normal[index2];
        Hcoord normal3 = normal[index3];
    	
        tempVerticesData.push_back(Vertex{
            Vector3{point1.x, point1.y, point1.z},
            Vector3{normal1.x, normal1.y, normal1.z}
        });

        tempVerticesData.push_back(Vertex{
		    Vector3{point2.x, point2.y, point2.z},
		    Vector3{normal2.x, normal2.y, normal2.z}
        });

        tempVerticesData.push_back(Vertex{
		    Vector3{point3.x, point3.y, point3.z},
		    Vector3{normal3.x, normal3.y, normal3.z}
        });
    }
	
    InitializeVerticesDatas();
}

std::vector<Vertex> SnubDodecMesh::GetTemp()
{
    return tempVerticesData;
}
