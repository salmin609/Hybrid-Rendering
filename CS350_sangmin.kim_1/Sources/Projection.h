/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Projection.h
Purpose: Return matrix for displaying on screen.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#ifndef CS250_PROJECTION_H
#define CS250_PROJECTION_H

#include "Camera.h"


Affine CameraToWorld(const Camera& cam);
Affine WorldToCamera(const Camera& cam);

Matrix Shadow(Point L);

Matrix CameraToNDC(const Camera& cam);
Matrix NDCToCamera(const Camera& cam);

Matrix DeviceToNdc(int width, int height);
Affine NDCToDevice(int width, int height);

Affine WorldToCameraWithoutTranslation(const Camera& cam);
Affine CameraToWorldWithoutTranslation(const Camera& cam);

#endif

