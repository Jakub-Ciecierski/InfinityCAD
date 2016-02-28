//
// Created by jakub on 2/28/16.
//

#include <geometry/object.h>
#include <drawer.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Object::Object() {
    position = vec3(0.0f);
    rotationAngles = vec3(0.0f);
    scaleFactor = 1.0f;
}

Object::~Object() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

mat4 Object::getTranslationMatrix() {
    mat4 translateMatrix = mat4(1.0f);

    translateMatrix[0].w = position.x;
    translateMatrix[1].w = position.y;
    translateMatrix[2].w = position.z;

    return translateMatrix;
}

mat4 Object::getRotationMatrix() {
    mat4 rotationMatrix
            = getXRotationMatrix() * getYRotationMatrix() * getZRotationMatrix();
    return rotationMatrix;
}

mat4 Object::getXRotationMatrix() {
    mat4 xRotateMatrix = mat4(1.0f);

    xRotateMatrix[1].y = cos(angleToRadians(rotationAngles.x));
    xRotateMatrix[1].z = -sin(angleToRadians(rotationAngles.x));
    xRotateMatrix[2].y = sin(angleToRadians(rotationAngles.x));
    xRotateMatrix[2].z = cos(angleToRadians(rotationAngles.x));

    return xRotateMatrix;
}

mat4 Object::getYRotationMatrix() {
    mat4 yRotateMatrix = mat4(1.0f);

    yRotateMatrix[0].x = cos(angleToRadians(rotationAngles.y));
    yRotateMatrix[0].z = sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].x = -sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].z = cos(angleToRadians(rotationAngles.y));

    return yRotateMatrix;
}

mat4 Object::getZRotationMatrix() {
    mat4 zRotateMatrix = mat4(1.0f);

    zRotateMatrix[0].x = cos(angleToRadians(rotationAngles.z));
    zRotateMatrix[0].y = -sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].x = sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].y = cos(angleToRadians(rotationAngles.z));

    return zRotateMatrix;
}

mat4 Object::getScaleMatrix() {
    mat4 scaleMatrix = mat4(1.0f);

    scaleMatrix[0].x = scaleFactor;
    scaleMatrix[1].y = scaleFactor;
    scaleMatrix[2].z = scaleFactor;

    return scaleMatrix;
}

//-----------------------------------------------------------//
//  PROTECTED METHODS
//-----------------------------------------------------------//

const mat4& Object::getModelMatrix() {
    this->modelMatrix
            = getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
    return modelMatrix;
}

void Object::clampAngles() {
    rotationAngles.x = rotationAngles.x < 0 ? 0 : rotationAngles.x;
    rotationAngles.y = rotationAngles.y < 0 ? 0 : rotationAngles.y;
    rotationAngles.z = rotationAngles.z < 0 ? 0 : rotationAngles.z;

    rotationAngles.x = rotationAngles.x > 360 ? 360 : rotationAngles.x;
    rotationAngles.y = rotationAngles.y > 360 ? 360 : rotationAngles.y;
    rotationAngles.z = rotationAngles.z > 360 ? 360 : rotationAngles.z;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void Object::move(float dx, float dy, float dz) {
    position.x += dx;
    position.y += dy;
    position.z += dz;
}

void Object::moveTo(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}


void Object::rotate(float dxAngle, float dyAngle, float dzAngle) {
    rotationAngles.x += dxAngle;
    rotationAngles.y += dyAngle;
    rotationAngles.z += dzAngle;

    clampAngles();
}

void Object::scale(float scale) {
    this->scaleFactor = scale;
}