#include <camera.h>

#include "utilities/HelperVec3.h"

// Static Move speeds
const float Camera::ZOOM_SPEED = 0.008f;


Camera::Camera(const uint16_t width, const uint16_t height) :
    _eye(0.0f, 3.0f, 10.0f),
    _lookAt(0.0f, 3.0f, 0.0f),
    _up(0.0f, 1.0f, 0.0f),

    _width(width),
    _height(height),
    _zNear(0.1f),
    _zFar(200.0f),

    _fovX(glm::radians(80.f))
{
    computeAspect();
    computeFovY();
    computeAxes();
}


void Camera::computeAspect()
{
    _aspect = (float)_width / _height;
}

void Camera::computeFovY()
{
    //compute fovY from fovX
    _fovY = 2.0f * glm::atan(glm::tan(_fovX / 2.0f) / _aspect);
}

void Camera::computeAxes()
{
    _n = glm::normalize(_eye - _lookAt);

    if (Vector3Helper::equals(_n, -_up)) {
        _u = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), _n));
    }
    else if (Vector3Helper::equals(_n, _up)) {
        _u = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), _n));
    }
    else {
        _u = glm::normalize(glm::cross(_up, _n));
    }

    _v = glm::normalize(glm::cross(_n, _u));
}


glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(_eye, _lookAt, _up);
}

glm::mat4 Camera::getProjectionMatrix()
{
    //Use perspective projection for now
    //TODO: Add ortho projection
    return glm::perspective(_fovY, _aspect, _zNear, _zFar);
}


void Camera::sizeChanged(const uint16_t width, const uint16_t height)
{
    _width = width;
    _height = height;
    computeAspect();
    computeFovY();
}



void Camera::orbitXZ(float offset)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), offset, _u);

    _eye = glm::vec3(rotationMatrix * glm::vec4(_eye - _lookAt, 1.0f)) + _lookAt;
    _u = glm::vec3(rotationMatrix * glm::vec4(_u, 0.0f));
    _v = glm::vec3(rotationMatrix * glm::vec4(_v, 0.0f));
    _n = glm::vec3(rotationMatrix * glm::vec4(_n, 0.0f));
    _up = glm::normalize(glm::cross(_n, _u));
}

void Camera::orbitY(float offset)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), offset, glm::vec3(0.0, 1.0f, 0.0f));

    _u = glm::vec3(rotationMatrix * glm::vec4(_u, 0.0f));
    _v = glm::vec3(rotationMatrix * glm::vec4(_v, 0.0f));
    _n = glm::vec3(rotationMatrix * glm::vec4(_n, 0.0f));
    _up = glm::normalize(glm::cross(_n, _u));
    _eye = glm::vec3(rotationMatrix * glm::vec4(_eye - _lookAt, 1.0f)) + _lookAt;
}

void Camera::dolly(float offset)
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -_n * offset);

    _eye = glm::vec3(translationMatrix * glm::vec4(_eye, 1.0f));
    _lookAt = glm::vec3(translationMatrix * glm::vec4(_lookAt, 1.0f));
}

void Camera::truck(float offset)
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _u * offset);				// Translation in x axis

    _eye = glm::vec3(translationMatrix * glm::vec4(_eye, 1.0f));
    _lookAt = glm::vec3(translationMatrix * glm::vec4(_lookAt, 1.0f));
}

void Camera::boom(float offset)
{
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _v * offset);			// Translation in y axis

    _eye = glm::vec3(translationMatrix * glm::vec4(_eye, 1.0f));
    _lookAt = glm::vec3(translationMatrix * glm::vec4(_lookAt, 1.0f));
}

void Camera::crane(float offset)
{
    boom(-offset);
}

void Camera::pan(float offset)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), offset, glm::vec3(0.0f, 1.0f, 0.0f));

    _u = glm::vec3(rotationMatrix * glm::vec4(_u, 0.0f));
    _v = glm::vec3(rotationMatrix * glm::vec4(_v, 0.0f));
    _n = glm::vec3(rotationMatrix * glm::vec4(_n, 0.0f));
    _up = glm::normalize(glm::cross(_n, _u));
    _lookAt = glm::vec3(rotationMatrix * glm::vec4(_lookAt - _eye, 1.0f)) + _eye;
}

void Camera::tilt(float offset)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), offset, _u);

    glm::vec3 n = glm::vec3(rotationMatrix * glm::vec4(_n, 0.0f));
    float alpha = glm::acos(glm::dot(n, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (alpha < offset || alpha >(glm::pi<float>() - offset))
    {
        return;
    }

    _v = glm::vec3(rotationMatrix * glm::vec4(_v, 0.0f));
    _n = n;
    _up = glm::normalize(glm::cross(_n, _u));											// It could change because of the rotation
    _lookAt = glm::vec3(rotationMatrix * glm::vec4(_lookAt - _eye, 1.0f)) + _eye;
}

void Camera::zoom(float offset)
{
    float newAngle = _fovY - (offset * ZOOM_SPEED);

    if (newAngle < glm::pi<float>() && newAngle > 0.0f) {
        _fovY = newAngle;
    }
}