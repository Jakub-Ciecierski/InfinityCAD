//
// Created by jakub on 2/28/16.
//

#include <gm/util/angle_to_radians.h>
#include <gm/rigid_body.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

RigidBody::RigidBody() {
    position = vec3(0.0f);
    rotationAngles = vec3(0.0f);
    scaleFactor = 1.0f;
}

RigidBody::~RigidBody() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

mat4 RigidBody::constructTranslationMatrix() {
    mat4 translateMatrix = mat4(1.0f);

    translateMatrix[3].x = position.x;
    translateMatrix[3].y = position.y;
    translateMatrix[3].z = position.z;

    return translateMatrix;
}

mat4 RigidBody::constructRotationMatrix() {
    mat4 rotationMatrix
            = constructXRotationMatrix() * constructYRotationMatrix() *
                    constructZRotationMatrix();
    return rotationMatrix;
}

mat4 RigidBody::constructXRotationMatrix() {
    mat4 xRotateMatrix = mat4(1.0f);

    xRotateMatrix[1].y = cos(angleToRadians(rotationAngles.x));
    xRotateMatrix[1].z = sin(angleToRadians(rotationAngles.x));

    xRotateMatrix[2].y = -sin(angleToRadians(rotationAngles.x));
    xRotateMatrix[2].z = cos(angleToRadians(rotationAngles.x));

    return xRotateMatrix;
}

mat4 RigidBody::constructYRotationMatrix() {
    mat4 yRotateMatrix = mat4(1.0f);

    yRotateMatrix[0].x = cos(angleToRadians(rotationAngles.y));
    yRotateMatrix[0].z = -sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].x = sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].z = cos(angleToRadians(rotationAngles.y));
    return yRotateMatrix;
}

mat4 RigidBody::constructZRotationMatrix() {
    mat4 zRotateMatrix = mat4(1.0f);

    zRotateMatrix[0].x = cos(angleToRadians(rotationAngles.z));
    zRotateMatrix[0].y = sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].x = -sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].y = cos(angleToRadians(rotationAngles.z));
    return zRotateMatrix;
}

mat4 RigidBody::constructScaleMatrix() {
    mat4 scaleMatrix = mat4(1.0f);

    scaleMatrix[0].x = scaleFactor;
    scaleMatrix[1].y = scaleFactor;
    scaleMatrix[2].z = scaleFactor;
    return scaleMatrix;
}

//-----------------------------------------------------------//
//  PROTECTED METHODS
//-----------------------------------------------------------//

const mat4& RigidBody::getModelMatrix() {
    return modelMatrix;
}

void RigidBody::clampAngles() {
    // TODO check for overflow
    /*
    rotationAngles.x = rotationAngles.x < 0 ? 0 : rotationAngles.x;
    rotationAngles.y = rotationAngles.y < 0 ? 0 : rotationAngles.y;
    rotationAngles.z = rotationAngles.z < 0 ? 0 : rotationAngles.z;

    rotationAngles.x = rotationAngles.x > 360 ? 360 : rotationAngles.x;
    rotationAngles.y = rotationAngles.y > 360 ? 360 : rotationAngles.y;
    rotationAngles.z = rotationAngles.z > 360 ? 360 : rotationAngles.z;
     */
}

void RigidBody::translate(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void RigidBody::move(float dx, float dy, float dz) {
    translate( dx, dy, dz);
}

void RigidBody::moveTo(float x, float y, float z) {
    translate(-position.x + x, -position.y + y, -position.z + z);
}

void RigidBody::moveTo(const vec3& pos) {
    translate(-position.x + pos.x, -position.y + pos.y, -position.z + pos.z);
}

void RigidBody::moveTo(const RigidBody* toBody) {
    vec3 pos = toBody->getPosition();
    translate(-position.x + pos.x, -position.y + pos.y, -position.z + pos.z);
}

void RigidBody::rotate(float dxAngle, float dyAngle, float dzAngle) {
    rotationAngles.x += dxAngle;
    rotationAngles.y += dyAngle;
    rotationAngles.z += dzAngle;

    clampAngles();
}

void RigidBody::scale(float scale) {
    this->scaleFactor = scale;
}

void RigidBody::scaleDt(float scale) {
    this->scaleFactor += scale;
    if(this->scaleFactor < 0.1)
        this->scaleFactor = 0.1;
}

const vec3& RigidBody::getPosition() const {
    return this->position;
}

void RigidBody::update() {
    this->modelMatrix = constructTranslationMatrix() *
                        constructRotationMatrix() *
                        constructScaleMatrix();
}