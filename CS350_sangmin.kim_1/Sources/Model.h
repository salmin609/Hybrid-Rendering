#pragma once
#include "Affine.h"

class Model
{
public:
	Model(Point modelPos = Point{0.f, 0.f, 0.f}, Point scale = Point{1.f, 1.f, 1.f}, float rotR = 0.f, 
		Vector rotA = Vector{0.f, 0.f, 1.f});
	Model(float pos, float scale);
	void ChangePos(Point newPos);
	Matrix GetModelMatNotTransposed();
	Matrix GetModelMatTransposed();
private:
	Point scaleVal;
	Point position;
	float rotRate;
	Vector rotAxis;
};


inline Model::Model(Point modelPos, Point scale, float rotR, Vector rotA) : position(modelPos), scaleVal(scale), rotRate(rotR), rotAxis(rotA)
{
}

inline Model::Model(float pos, float scale) : position(Point{pos, pos, pos}), scaleVal(Point{scale, scale, scale }), rotRate(0.f), rotAxis(Vector{0.f, 0.f, 1.f})
{
}

inline void Model::ChangePos(Point newPos)
{
	position = newPos;
}

inline Matrix Model::GetModelMatNotTransposed()
{
	return translate(position - Point(0, 0, 0))* rotate(rotRate, rotAxis)* scale(scaleVal.x, scaleVal.y, scaleVal.z);
}

inline Matrix Model::GetModelMatTransposed()
{
	return transpose(GetModelMatNotTransposed());
}
