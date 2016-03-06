//
// Created by jakub on 3/5/16.
//

#include <glm/gtc/matrix_transform.hpp>
#include <gm/cameras/camera_fps.h>
#include <gm/angle_to_radians.h>
#include <iostream>

using namespace glm;

CameraFPS::CameraFPS(Projection *projection) :
        Camera(projection) {
    buttonsSpeed = 0.15;
    mouseSpeed = 0.06;
}

CameraFPS::~CameraFPS() {

}

const glm::mat4 &CameraFPS::getVPMatrix() {


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

    mat4 viewM = mat4();
    viewM[0].x = X.x;
    viewM[1].x = X.y;
    viewM[2].x = X.z;
    viewM[3].x = (X.x * -position.x + X.y * -position.y + X.z * -position.z);

    viewM[0].y = up.x;
    viewM[1].y = up.y;
    viewM[2].y = up.z;
    viewM[3].y = (up.x * -position.x + up.y * -position.y + up.z * -position.z);

    viewM[0].z = direction.x;
    viewM[1].z = direction.y;
    viewM[2].z = direction.z;
    viewM[3].z = (direction.x * -position.x +
                   direction.y * -position.y +
                   direction.z * -position.z);
    viewM[0].w = 0;
    viewM[1].w = 0;
    viewM[2].w = 0;
    viewM[3].w = 1.0f;

    VP = projection->getProjectionMatrix() * viewM;

    return VP;

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
    position -= right * (buttonsSpeed / 2) * speedBoost;
}

void CameraFPS::moveRight(float speedBoost) {
    position += right * (buttonsSpeed / 2) * speedBoost;
}

void CameraFPS::moveUp(float speedBoost) {
    position += up * (buttonsSpeed * speedBoost);
}

void CameraFPS::moveDown(float speedBoost) {
    position -= up * (buttonsSpeed * speedBoost);
}
