// Affine.h
// -- points, vectors, transformations in 3D
// cs250 8/17
#pragma once

#ifndef CS250_AFFINE_H
#define CS250_AFFINE_H

#include <cmath>

#include <cassert>
#include <vector>
struct Vector3;

struct Hcoord {
	float x, y, z, w;
	explicit Hcoord(float X = 0, float Y = 0, float Z = 0, float W = 0);
	float& operator[](int i) { return *(&x + i); }
	float operator[](int i) const { return *(&x + i); }
	void operator+=(const Hcoord& A);
	void operator/=(const float val);
	static bool near(float x, float y) { return std::abs(x - y) < 1e-5f; }
};


struct Point : Hcoord {
	explicit Point(float X = 0, float Y = 0, float Z = 0);
	Point(const Hcoord& v) : Hcoord(v) { }
};


struct Vector : Hcoord {
	explicit Vector(float X = 0, float Y = 0, float Z = 0);
	Vector(const Hcoord& v) : Hcoord(v) { }
	Vector operator*(float val)
	{
		return Vector(x * val, y * val, z * val);
	}

	Vector operator/=(float val)
	{
		return Vector(x /= val, y /= val, z /= val);
	}
	bool operator<(Vector val)
	{
		return z < val.z;
	}
};


struct Matrix {
	Hcoord row[4];
	Hcoord& operator[](int i) { return row[i]; }
	const Hcoord& operator[](int i) const { return row[i]; }
	Hcoord operator*(Hcoord val)
	{
		Hcoord result;
		result.x = row[0].x * val.x + row[0].y * val.y + row[0].z * val.z + row[0].w * val.w;
		result.y = row[1].x * val.x + row[1].y * val.y + row[1].z * val.z + row[1].w * val.w;
		result.z = row[2].x * val.x + row[2].y * val.y + row[2].z * val.z + row[2].w * val.w;
		result.w = row[3].x * val.x + row[3].y * val.y + row[3].z * val.z + row[3].w * val.w;
		return result;
	}
};


struct Affine : Matrix {
	Affine(void);
	Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D);
	Affine(const Matrix& M) : Matrix(M)
	{
		/*assert(Hcoord::near(M[3][0], 0) && Hcoord::near(M[3][1], 0)
			&& Hcoord::near(M[3][2], 0) && Hcoord::near(M[3][3], 1));*/
	}
};


Hcoord operator+(const Hcoord& u, const Hcoord& v);
Hcoord operator-(const Hcoord& u, const Hcoord& v);
Hcoord operator-(const Hcoord& v);
Hcoord operator*(float r, const Hcoord& v);
Hcoord operator*(const Matrix& A, const Hcoord& v);
Matrix operator*(const Matrix& A, const Matrix& B);
float dot(const Vector& u, const Vector& v);
float abs(const Vector& v);
Vector normalize(const Vector& v);
Vector cross(const Vector& u, const Vector& v);
Affine rotate(float t, const Vector& v);
Affine translate(const Vector& v);
Affine scale(float r);
Affine scale(float rx, float ry, float rz);
Affine inverse(const Affine& A);
Matrix transpose(const Matrix& A);
Affine identityMatrix();

struct Vector2
{
	void operator+=(Hcoord val)
	{
		x += val.x;
		y += val.y;
	}
	bool operator!=(Vector2 val)
	{
		return x != val.x && y != val.y;
	}
	float x, y;
};
struct Vector3
{
	Vector3() : x(0.f), y(0.f), z(0.f)
	{
	}
	Vector3(Point val) : x(val.x), y(val.y), z(val.z)
	{
	}
	Vector3(float val)
	{
		x = val;
		y = val;
		z = val;
	}
	Vector3(float valX, float valY, float valZ)
	{
		x = valX;
		y = valY;
		z = valZ;
	}
	void operator+=(Hcoord val)
	{
		x += val.x;
		y += val.y;
		z += val.z;
	}
	void operator+=(Vector3 val)
	{
		x += val.x;
		y += val.y;
		z += val.z;
	}

	bool operator!=(Vector3 val)
	{
		return x != val.x || y != val.y || z != val.z;
	}
	Vector3 operator+(Vector3 val)
	{
		Vector3 result{};
		result.x = x + val.x;
		result.y = y + val.y;
		result.z = z + val.z;
		return result;
	}
	
	Vector3 operator-(float val)
	{
		Vector3 result{};
		result.x = x - val;
		result.y = y - val;
		result.z = z - val;
		return result;
	}

	Vector3 operator-(Vector3 val)
	{
		Vector3 result{};
		result.x = x - val.x;
		result.y = y - val.y;
		result.z = z - val.z;
		return result;
	}

	Vector3 operator*(Vector3 val)
	{
		Vector3 result{};
		result.x = x * val.x;
		result.y = y * val.y;
		result.z = z * val.z;
		return result;
	}
	Vector3 operator*(float val)
	{
		Vector3 result{};
		result.x = x * val;
		result.y = y * val;
		result.z = z * val;
		return result;
	}
	float x, y, z;
};
struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 texCoord;
	Vector3 color;
};
struct Vector4
{
	Vector4() : x(0.f), y(0.f), z(0.f), w(1.f)
	{
	}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{
	}
	Vector4(float val) : x(val), y(val), z(val), w(1.f)
	{
	}

	void operator+=(Vector4 val)
	{
		x += val.x;
		y += val.y;
		z += val.z;
	}
	void operator=(Vector3 val)
	{
		x = val.x;
		y = val.y;
		z = val.z;
		w = 1.f;
	}
	Vector4 operator-(Vector4 val)
	{
		Vector4 result;
		result.x = x - val.x;
		result.y = y - val.y;
		result.z = z - val.z;

		return result;
	}
	
	float x, y, z, w;
};


inline float degrees(float angleInRadians)
{
	const float PI = 3.14159265358979323846f;
	return angleInRadians * 180.0f / PI;
}

inline float radians(float angleInDegrees)
{
	const float PI = 3.14159265358979323846f;
	return angleInDegrees * (PI / 180.0f);
}
struct ivec3
{
	int x = 0, y = 0, z = 0;
};
class AssimpVertexData
{
public:
	Vector3 pnt;
	Vector3 nrm;
	Vector2 tex;
	Vector3 tan;
	AssimpVertexData(const Vector3& p, const Vector3& n, const Vector2& t, const Vector3& a)
		: pnt(p), nrm(n), tex(t), tan(a)
	{}
};

class AssimpMaterial
{
public:
	Vector3 Kd, Ks;
	float alpha;
	unsigned int texid;

	virtual bool isLight() { return false; }

	AssimpMaterial() : Kd(Vector3(1.0, 0.5, 0.0)), Ks(Vector3(1, 1, 1)), alpha(1.0), texid(0) {}
	AssimpMaterial(const Vector3 d, const Vector3 s, const float a)
		: Kd(d), Ks(s), alpha(a), texid(0) {}
	AssimpMaterial(AssimpMaterial& o) { Kd = o.Kd;  Ks = o.Ks;  alpha = o.alpha;  texid = o.texid; }

	//virtual void apply(const unsigned int program);
};

struct AssimpMeshData
{
	std::vector<AssimpVertexData> vertices;
	std::vector<ivec3> triangles;
	AssimpMaterial* mat;
};
#endif

