//
// Created by jakub on 2/28/16.
//

#include <gm/object.h>
#include <gm/angle_to_radians.h>

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

mat4 Object::constructTranslationMatrix() {
    mat4 translateMatrix = mat4(1.0f);

    translateMatrix[3].x = position.x;
    translateMatrix[3].y = position.y;
    translateMatrix[3].z = position.z;

    return translateMatrix;
}

mat4 Object::constructRotationMatrix() {
    mat4 rotationMatrix
            = constructXRotationMatrix() * constructYRotationMatrix() *
                    constructZRotationMatrix();
    return rotationMatrix;
}

mat4 Object::constructXRotationMatrix() {
    mat4 xRotateMatrix = mat4(1.0f);

    xRotateMatrix[1].y = cos(angleToRadians(rotationAngles.x));
    xRotateMatrix[1].z = sin(angleToRadians(rotationAngles.x));

    xRotateMatrix[2].y = -sin(angleToRadians(rotationAngles.x));
    xRotateMatrix[2].z = cos(angleToRadians(rotationAngles.x));

    return xRotateMatrix;
}

mat4 Object::constructYRotationMatrix() {
    mat4 yRotateMatrix = mat4(1.0f);

    yRotateMatrix[0].x = cos(angleToRadians(rotationAngles.y));
    yRotateMatrix[0].z = -sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].x = sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].z = cos(angleToRadians(rotationAngles.y));
    return yRotateMatrix;
}

mat4 Object::constructZRotationMatrix() {
    mat4 zRotateMatrix = mat4(1.0f);

    zRotateMatrix[0].x = cos(angleToRadians(rotationAngles.z));
    zRotateMatrix[0].y = sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].x = -sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].y = cos(angleToRadians(rotationAngles.z));
    return zRotateMatrix;
}

mat4 Object::constructScaleMatrix() {
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
    return modelMatrix;
}

void Object::clampAngles() {
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

void Object::scaleDt(float scale) {
    this->scaleFactor += scale;
    if(this->scaleFactor < 0.1)
        this->scaleFactor = 0.1;
}

const vec3& Object::getPosition() {
    return this->position;
}

void Object::update() {
    this->modelMatrix = constructTranslationMatrix() *
                        constructRotationMatrix() *
                        constructScaleMatrix();
}