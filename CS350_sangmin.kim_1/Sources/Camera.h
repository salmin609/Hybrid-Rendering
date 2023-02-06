// Camera.h
// -- 3D camera class
// cs250 1/15

#ifndef CS250_CAMERA_H
#define CS250_CAMERA_H

#include "Affine.h"


class Camera {
public:
    Camera(void);
    Camera(const Point& E, const Vector& look, const Vector& vp,
        float fov, float aspect, float near, float far);
    Point Eye(void) const;
    Vector Right(void) const;
    Vector Up(void) const;
    Vector Back(void) const;
    Vector ViewportGeometry(void) const;
    float NearDistance(void) const;
    float FarDistance(void) const;
    Camera& Zoom(float factor);
    Camera& Forward(float distance_increment);
    Camera& Yaw(float angle);
    Camera& Pitch(float angle);
    Camera& Roll(float angle);
    void Switch(int i);
    void ChangeCameraYPosition(float distance);
    void InvertPitch();
    float Get_Width() const 
    {
        return width;
    }
    float Get_Height() const
    {
        return height;
    }
private:
    Point eye;
    Vector right, up, back;
    float width, height, distance,
        near, far;

};


#endif

