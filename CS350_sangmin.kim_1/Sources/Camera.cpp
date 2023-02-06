/*************************************************************
 *	Author				: Kim sang min.
 *	Assignment number	: Project assignment
 *	Course				: CS250.
 *  Term				: Spring 2020.
 *************************************************************/

#include "Camera.h"

//Initialize camera
Camera::Camera() : right(1.f,0.f,0.f), up(0.f, 1.f, 0.f), back(0.f, 0.f, 1.f), width(10.f), height(10.f), distance(5.f), near(0.001f), far(10.f)
{
}

Camera::Camera(const Point& E, const Vector& look, const Vector& vp, float fov, float aspect, float near, float far) : eye(E), near(near), far(far)
{
	const Vector l_cross_r = cross(look, vp);
	const float tan_val = tan(fov / 2);
	
	back = -1 / abs(look) * look;
	right = 1 / abs(l_cross_r) * l_cross_r;
	up = cross(back, right);

	distance = near;
	width = (tan_val * distance) * 2;
	height = width / aspect;
}

Point Camera::Eye() const
{
	return eye;
}

Vector Camera::Right() const
{
	return right;
}

Vector Camera::Up() const
{
	return up;
}

Vector Camera::Back() const
{
	return back;
}

Vector Camera::ViewportGeometry() const
{
	return Vector(width, height, distance);
}

float Camera::NearDistance() const
{
	return near;
}

float Camera::FarDistance() const
{
	return far;
}

Camera& Camera::Zoom(float factor)
{
	width *= factor;
	height *= factor;

	return *this;
}

Camera& Camera::Forward(float distance_increment)
{
	eye.x -= (distance_increment * back.x);
	eye.y -= (distance_increment * back.y);
	eye.z -= (distance_increment * back.z);
	distance += distance_increment;
	
	return *this;
}

Camera& Camera::Yaw(float angle)
{
	const Affine rotation_mat = rotate(angle, up);
	back = rotation_mat * back;
	right = rotation_mat * right;

	return *this;
}

Camera& Camera::Pitch(float angle)
{
	const Affine rotation_mat = rotate(angle, right);
	back = rotation_mat * back;
	up = rotation_mat * up;

	return *this;
}

Camera& Camera::Roll(float angle)
{
	const Affine rotation_mat = rotate(angle, back);
	up = rotation_mat * up;
	right = rotation_mat * right;

	return *this;
}

void Camera::Switch(int i)
{
	switch(i)
	{
	case 0:
		Pitch(0.f);
		Yaw(90.f);
		break;
	case 1:
		Pitch(0.f);
		Yaw(-90.f);
		break;
	case 2:
		Pitch(-90.f);
		Yaw(180.f);
		break;
	case 3:
		Pitch(90.f);
		Yaw(180.f);
		break;
	case 4:
		Pitch(0.f);
		Yaw(180.f);
		break;
	case 5:
		Pitch(0.f);
		Yaw(0.f);
		break;

	default: ;
	}
}

void Camera::ChangeCameraYPosition(float downDistance)
{
	eye.y -= downDistance;
	InvertPitch();
}

void Camera::InvertPitch()
{
	*this = Pitch(180.f);
}
