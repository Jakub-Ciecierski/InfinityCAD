//
// Created by jakub on 3/5/16.
//

#include <glm/gtc/matrix_transform.hpp>
#include <gm/cameras/camera_fps.h>
#include <gm/util/angle_to_radians.h>
#include <iostream>

using namespace glm;

CameraFPS::CameraFPS() :
        Camera(){
    update();
}


CameraFPS::CameraFPS(Projection *projection) :
        Camera(projection) {
    buttonsSpeed = 0.15;
    mouseSpeed = 0.06;

    verticalAngleDegree = 0;
    horizontalAngleDegree = 0;
}

CameraFPS::~CameraFPS() {

}

void CameraFPS::moveForward() {
    moveForward(1.0f);
}

void CameraFPS::moveBackward() {
    moveBackward(1.0f);
}

void CameraFPS::moveLeft() {
    moveLeft(1.0f);
}

void CameraFPS::moveRight() {
    moveRight(1.0f);
}

void CameraFPS::moveUp() {
    moveUp(1.0f);
}

void CameraFPS::moveDown() {
    moveDown(1.0f);
}


void CameraFPS::moveForward(float speedBoost) {
    position += direction * buttonsSpeed * speedBoost;
}

void CameraFPS::moveBackward(float speedBoost) {
    position -= direction * buttonsSpeed * speedBoost;
}

void CameraFPS::moveLeft(float speedBoost) {
    position -= right * (buttonsSpeed ) * speedBoost;
}

void CameraFPS::moveRight(float speedBoost) {
    position += right * (buttonsSpeed / 1) * speedBoost;
}

void CameraFPS::moveUp(float speedBoost) {
    position += up * (buttonsSpeed * speedBoost);
}

void CameraFPS::moveDown(float speedBoost) {
    position -= up * (buttonsSpeed * speedBoost);
}

const vec3& CameraFPS::getDirection(){
    return this->direction;
}

const mat4 &CameraFPS::getViewMatrix() {
    return viewMatrix;
}

const mat4 &CameraFPS::getVPMatrix() {
    return VP;

}

void CameraFPS::update() {
    direction = vec3(
            cos(angleToRadians(verticalAngleDegree)) *
            sin(angleToRadians(horizontalAngleDegree)),
            sin(angleToRadians(verticalAngleDegree)),
            cos(angleToRadians(verticalAngleDegree)) *
            cos(angleToRadians(horizontalAngleDegree)));

    right = vec3(sin(angleToRadians(horizontalAngleDegree) - M_PI_2),
                 0,
                 cos(angleToRadians(horizontalAngleDegree) - M_PI_2)
    );

    direction = normalize(direction);
    right = normalize(right);

    up = cross(right, direction);
    up = normalize(up);

    vec3 X = right;
    up = cross(direction, X);

    X = normalize(X);
    up = normalize(up);

    viewMatrix[0].x = X.x;
    viewMatrix[1].x = X.y;
    viewMatrix[2].x = X.z;
    viewMatrix[3].x = (X.x * -position.x + X.y * -position.y + X.z * -position.z);

    viewMatrix[0].y = up.x;
    viewMatrix[1].y = up.y;
    viewMatrix[2].y = up.z;
    viewMatrix[3].y = (up.x * -position.x + up.y * -position.y + up.z * -position.z);

    viewMatrix[0].z = direction.x;
    viewMatrix[1].z = direction.y;
    viewMatrix[2].z = direction.z;
    viewMatrix[3].z = (direction.x * -position.x +
                  direction.y * -position.y +
                  direction.z * -position.z);
    viewMatrix[0].w = 0;
    viewMatrix[1].w = 0;
    viewMatrix[2].w = 0;
    viewMatrix[3].w = 1.0f;

    VP = projection->getProjectionMatrix() * viewMatrix;
}