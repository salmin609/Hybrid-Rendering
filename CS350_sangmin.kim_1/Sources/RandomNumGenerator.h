/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: RandomNumGenerator.h
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#pragma once

#include <ctime>
#include <random>
#include "Affine.h"

class RandomNumber
{
public:
	static RandomNumber* instance;

	RandomNumber();
	float RandomFloat(float min, float max);
	int RandomInt(int max);
	Vector3 RandomVector3(float min, float max);

private:
	std::random_device rd;
};