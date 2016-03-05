//
// Created by jakub on 3/5/16.
//

#include <glm/gtc/matrix_transform.hpp>
#include <gm/cameras/camera_fps.h>
#include <gm/angle_to_radians.h>

using namespace glm;

CameraFPS::CameraFPS(Projection *projection) :
        Camera(projection) {
    buttonsSpeed = 0.5;
    mouseSpeed = 0.09;
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
                 cos(angleToRadians(horizontalAngleDegree) - M_PI_2));

    direction = normalize(direction);
    right = normalize(right);
    up = cross(right, direction);
    up = normalize(up);

    vec3 X = cross(up, direction);
    up = cross(direction, X);

    X = normalize(X);
    up = normalize(up);

    mat4 viewM = mat4();
    viewM[0][0] = X.x;
    viewM[1][0] = X.y;
    viewM[2][0] = X.z;
    viewM[3][0] = (X.x * position.x + X.y * position.y + X.z * position.z);
    viewM[0][1] = up.x;
    viewM[1][1] = up.y;
    viewM[2][1] = up.z;
    viewM[3][1] = (up.x * position.x + up.y * position.y + up.z * position.z);
    viewM[0][2] = direction.x;
    viewM[1][2] = direction.y;
    viewM[2][2] = direction.z;
    viewM[3][2] = (direction.x * position.x +
                   direction.y * position.y +
                   direction.z * position.z);
    viewM[0][3] = 0;
    viewM[1][3] = 0;
    viewM[2][3] = 0;
    viewM[3][3] = 1.0f;

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
    position -= direction * buttonsSpeed * speedBoost;
}

void CameraFPS::moveBackward(float speedBoost) {
    position += direction * buttonsSpeed * speedBoost;
}

void CameraFPS::moveLeft(float speedBoost) {
    position -= right * (buttonsSpeed / 4) * speedBoost;
}

void CameraFPS::moveRight(float speedBoost) {
    position += right * (buttonsSpeed / 4) * speedBoost;
}

void CameraFPS::moveUp(float speedBoost) {
    position += up * (buttonsSpeed * speedBoost);
}

void CameraFPS::moveDown(float speedBoost) {
    position -= up * (buttonsSpeed * speedBoost);
}
