#pragma once
#include "stdafx.h"

// A camera that looks at the origin and rotates by mouse movement.
class Camera
{

protected:
    glm::vec3 _eye;
    glm::vec3 _lookAt;
    glm::vec3 _up;

    glm::vec3 _n, _u, _v;

    uint16_t _width, _height;
    float _aspect;
    float _zNear, _zFar;
    float _fovX, _fovY;

protected:
    static const float ZOOM_SPEED;

public:

    Camera(const uint16_t width, const uint16_t height);
    
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void computeAspect();
    void computeFovY();
    void computeAxes();

    void sizeChanged(const uint16_t width, const uint16_t height);

    // [Camera Movements / Zoom]
    void orbitXZ(float offset);
    void orbitY(float offset);
    void dolly(float offset);
    void truck(float offset);
    void boom(float offset);
    void crane(float offset);
    void pan(float offset);
    void tilt(float offset);
    void zoom(float offset);

    // [Getters / Setters]
    uint16_t getWidth() const { return _width; }
    uint16_t getHeight() const { return _height; }
    glm::vec3 getEye() const { return _eye; }
    float getFovY() const { return _fovY; }
    float getZNear() const { return _zNear; }
    float getZFar() const { return _zFar; }
};
