/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "Affine.h"

constexpr int MAX_INDEX = 4;

/*
 * Helper functions.
 */
namespace
{
	/*
	 * Helper for '+' operator for the matrix.
	 */
	Affine operator+(const Affine& A, const Affine& B)
	{
		Affine result;

		for (int i = 0; i < MAX_INDEX - 1; i++)
		{
			for (int j = 0; j < MAX_INDEX; j++)
			{
				result[i][j] = A[i][j] + B[i][j];
			}
		}

		result[3][0] = 0.f;
		result[3][1] = 0.f;
		result[3][2] = 0.f;
		result[3][3] = 1.f;

		return result;
	}

	/*
	 * Helper for build & return the scale matrix.
	 * 2 overloaded version - vx, vy, vz
	 *						- val
	 */
	Affine Build_Scale_Mat(const float& vx, const float& vy, const float& vz)
	{
		Affine scale_mat;
		
		scale_mat[0][0] = vx;
		scale_mat[1][1] = vy;
		scale_mat[2][2] = vz;

		return scale_mat;
	}

	Affine Build_Scale_Mat(const float& val)
	{
		Affine scale_mat;

		scale_mat[0][0] = val;
		scale_mat[1][1] = val;
		scale_mat[2][2] = val;
		
		return scale_mat;
	}

	/*
	 * Helper for build & return translation matrix.
	 */
	Affine Build_Trans_Mat(const Vector& vec)
	{
		Affine trans_mat;

		trans_mat[0][0] = 1.f;
		trans_mat[1][1] = 1.f;
		trans_mat[2][2] = 1.f;
		
		trans_mat[0][3] = vec.x;
		trans_mat[1][3] = vec.y;
		trans_mat[2][3] = vec.z;

		return trans_mat;
	}

	/*
	 * Helper functions for building rotation matrix.
	 *
	 * Helper_Build_Rotation_First_Mat		= cos(theta) * Identity matrix.
	 * Helper_Build_Rotation_Second_Mat		= (1 - cos(theta) / length of vec squared) * (u * u(transposed)).
	 * Helper_Build_Rotation_Third_Mat		= (sin(theta) / length of vec) * cross product mat.
	 */
	Affine Helper_Build_Rotation_First_Mat(float angle)
	{
		const float cos_angle = cos(angle);

		return Build_Scale_Mat(cos_angle);
	}

	Affine Helper_Build_Rotation_Second_Mat(float angle, Vector vec)
	{
		const float cos_angle = cos(angle);
		const float val = (1 - cos_angle) / (abs(vec) * abs(vec));

		Affine mat;
		{
			mat[0] = Hcoord(vec.x * vec.x * val, vec.x * vec.y * val, vec.x * vec.z * val, vec.x * vec.w * val);
			mat[1] = Hcoord(vec.y * vec.x * val, vec.y * vec.y * val, vec.y * vec.z * val, vec.y * vec.w * val);
			mat[2] = Hcoord(vec.z * vec.x * val, vec.z * vec.y * val, vec.z * vec.z * val, vec.z * vec.w * val);
		}

		return mat;
	}
	
	Affine Helper_Build_Rotation_Third_Mat(float angle, Vector vec)
	{
		const float val = sin(angle) / abs(vec);

		Affine mat;
		{
			mat[0] = Hcoord(0.f, -vec.z * val, vec.y * val, 0.f);
			mat[1] = Hcoord(vec.z * val, 0.f, -vec.x * val, 0.f);
			mat[2] = Hcoord(-vec.y * val, vec.x * val, 0.f,0.f);
		}

		return mat;
	}

	/*
	 * Helper functions for building inversed matrix.
	 *
	 * Get_Det				= Helper for get determinant value.
	 * Helper_Inverse_Mat	= Use det value, inverse each elements. 
	 */
	float Get_Det(const Affine& mat)
	{
		return (mat[0][0] * ((mat[1][1] * mat[2][2]) - (mat[1][2] * mat[2][1]))) +
			(-mat[0][1] * ((mat[1][0] * mat[2][2]) - (mat[2][0] * mat[1][2]))) +
			(mat[0][2] * ((mat[1][0] * mat[2][1]) - (mat[1][1] * mat[2][0])));
	}

	Affine Helper_Inverse_Mat(const Affine& mat)
	{
		const float inverse_det = 1.f / Get_Det(mat);
		
		Affine inversed_linear_mat;

		inversed_linear_mat[0][0] = ((mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) * inverse_det);
		inversed_linear_mat[0][1] = ((mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * inverse_det);
		inversed_linear_mat[0][2] = ((mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * inverse_det);
		inversed_linear_mat[1][0] = ((mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * inverse_det);
		inversed_linear_mat[1][1] = ((mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * inverse_det);
		inversed_linear_mat[1][2] = ((mat[1][0] * mat[0][2] - mat[0][0] * mat[1][2]) * inverse_det);
		inversed_linear_mat[2][0] = ((mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]) * inverse_det);
		inversed_linear_mat[2][1] = ((mat[2][0] * mat[0][1] - mat[0][0] * mat[2][1]) * inverse_det);
		inversed_linear_mat[2][2] = ((mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]) * inverse_det);

		return inversed_linear_mat;
	}
}


Hcoord::Hcoord(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W)
{
}

void Hcoord::operator+=(const Hcoord& A)
{
	x += A.x;
	y += A.y;
	z += A.z;
}

void Hcoord::operator/=(const float val)
{
	x /= val;
	y /= val;
	z /= val;
}

Point::Point(float X, float Y, float Z) : Hcoord(X, Y, Z, 1.f)
{
}

Vector::Vector(float X, float Y, float Z) : Hcoord(X, Y, Z, 0.f)
{
}

Affine::Affine()
{
	row[3][3] = 1.f;
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
	row[0] = Hcoord(Lx.x, Ly.x, Lz.x, D.x);
	row[1] = Hcoord(Lx.y, Ly.y, Lz.y, D.y);
	row[2] = Hcoord(Lx.z, Ly.z, Lz.z, D.z);
	row[3] = Hcoord(Lx.w, Ly.w, Lz.w, D.w);
}

Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
	return Hcoord(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
	return Hcoord(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

Hcoord operator-(const Hcoord& v)
{
	return Hcoord(-v.x, -v.y, -v.z, -v.w);
}

Hcoord operator*(float r, const Hcoord& v)
{
	return Hcoord(r * v.x, r * v.y, r * v.z, r * v.w);
}

Hcoord operator*(const Matrix& A, const Hcoord& v)
{
	return Hcoord(
		A[0][0] * v.x + A[0][1] * v.y + A[0][2] * v.z + A[0][3] * v.w,
		A[1][0] * v.x + A[1][1] * v.y + A[1][2] * v.z + A[1][3] * v.w,
		A[2][0] * v.x + A[2][1] * v.y + A[2][2] * v.z + A[2][3] * v.w,
		A[3][0] * v.x + A[3][1] * v.y + A[3][2] * v.z + A[3][3] * v.w
	);
}

Matrix operator*(const Matrix& A, const Matrix& B)
{
	Matrix result;

	for (int i = 0; i < MAX_INDEX; ++i)
	{
		for (int j = 0; j < MAX_INDEX; ++j)
		{
			for (int k = 0; k < MAX_INDEX; ++k)
			{
				result[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	return result;
}



float dot(const Vector& u, const Vector& v)
{
	return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}


float abs(const Vector& v)
{
	return sqrt(dot(v,v));
}


Vector normalize(const Vector& v)
{
	const float abs_value = abs(v);
	
	return Vector(v.x / abs_value, v.y / abs_value, v.z / abs_value);
}


Vector cross(const Vector& u, const Vector& v)
{
	return Vector(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

Affine rotate(float t, const Vector& v)
{
	const Affine first_mat = Helper_Build_Rotation_First_Mat(t);
	const Affine second_mat = Helper_Build_Rotation_Second_Mat(t, v);
	const Affine last_mat = Helper_Build_Rotation_Third_Mat(t, v);
	
	return first_mat + second_mat + last_mat;
}

Affine translate(const Vector& v)
{
	return Build_Trans_Mat(v);
}

Affine scale(float r)
{
	return Build_Scale_Mat(r);
}

Affine scale(float rx, float ry, float rz)
{
	return Build_Scale_Mat(rx, ry, rz);
}

Affine inverse(const Affine& A)
{
	//Build inverse translate matrix.
	const Affine inversed_translate = translate(Vector(-A[0][3], -A[1][3], -A[2][3]));

	//Build inverse linear matrix.
	const Affine inversed_linear_mat = Helper_Inverse_Mat(A);

	return inversed_linear_mat * inversed_translate;
}

Matrix  transpose(const Matrix& A)
{
	Matrix result;

	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			result[i][j] = A[j][i];
		}
	}
	return result;
}

Affine identityMatrix()
{
	Affine result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result[i][j] = 0.f;

			if(i == j)
			{
				result[i][j] = 1.f;
			}
		}
	}
	return result;
}
