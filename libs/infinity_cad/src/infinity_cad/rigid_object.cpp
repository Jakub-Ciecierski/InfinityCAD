//
// Created by jakub on 2/28/16.
//


#include <infinity_cad/rigid_object.h>
#include <infinity_cad/math/math.h>

using namespace std;
using namespace ifc;
using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

RigidObject::RigidObject(SceneID id) :
    Object(id){
    position = vec3(0.0f);
    rotationAngles = vec3(0.0f);
    scaleFactor = 1.0f;
}
RigidObject::RigidObject(SceneID id, std::string name) :
    Object(id, name){
    position = vec3(0.0f);
    rotationAngles = vec3(0.0f);
    scaleFactor = 1.0f;
}

RigidObject::~RigidObject() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

mat4 RigidObject::constructTranslationMatrix() {
    mat4 translateMatrix = mat4(1.0f);

    translateMatrix[3].x = position.x;
    translateMatrix[3].y = position.y;
    translateMatrix[3].z = position.z;

    return translateMatrix;
}

mat4 RigidObject::constructRotationMatrix() {
    mat4 rotationMatrix
            = constructXRotationMatrix() * constructYRotationMatrix() *
                    constructZRotationMatrix();
    return rotationMatrix;
}

mat4 RigidObject::constructXRotationMatrix() {
    mat4 xRotateMatrix = mat4(1.0f);

    xRotateMatrix[1].y = cos(angleToRadians(rotationAngles.x));
    xRotateMatrix[1].z = sin(angleToRadians(rotationAngles.x));

    xRotateMatrix[2].y = -sin(angleToRadians(rotationAngles.x));
    xRotateMatrix[2].z = cos(angleToRadians(rotationAngles.x));

    return xRotateMatrix;
}

mat4 RigidObject::constructYRotationMatrix() {
    mat4 yRotateMatrix = mat4(1.0f);

    yRotateMatrix[0].x = cos(angleToRadians(rotationAngles.y));
    yRotateMatrix[0].z = -sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].x = sin(angleToRadians(rotationAngles.y));
    yRotateMatrix[2].z = cos(angleToRadians(rotationAngles.y));
    return yRotateMatrix;
}

mat4 RigidObject::constructZRotationMatrix() {
    mat4 zRotateMatrix = mat4(1.0f);

    zRotateMatrix[0].x = cos(angleToRadians(rotationAngles.z));
    zRotateMatrix[0].y = sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].x = -sin(angleToRadians(rotationAngles.z));
    zRotateMatrix[1].y = cos(angleToRadians(rotationAngles.z));
    return zRotateMatrix;
}

mat4 RigidObject::constructScaleMatrix() {
    mat4 scaleMatrix = mat4(1.0f);

    scaleMatrix[0].x = scaleFactor;
    scaleMatrix[1].y = scaleFactor;
    scaleMatrix[2].z = scaleFactor;
    return scaleMatrix;
}

//-----------------------------------------------------------//
//  PROTECTED METHODS
//-----------------------------------------------------------//

void RigidObject::clampAngles() {
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

void RigidObject::translate(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void RigidObject::move(float dx, float dy, float dz) {
    translate( dx, dy, dz);
}

void RigidObject::moveTo(float x, float y, float z) {
    translate(-position.x + x, -position.y + y, -position.z + z);
}

void RigidObject::moveTo(const vec3& pos) {
    float x = -position.x + pos.x;
    float y = -position.y + pos.y;
    float z = -position.z + pos.z;
    translate(x, y, z);
}

void RigidObject::moveTo(const RigidObject * toBody) {
    vec3 pos = toBody->getPosition();
    translate(-position.x + pos.x, -position.y + pos.y, -position.z + pos.z);
}

void RigidObject::rotateTo(float xAngle, float yAngle, float zAngle){
    rotationAngles.x = xAngle;
    rotationAngles.y = yAngle;
    rotationAngles.z = zAngle;

    clampAngles();
}

void RigidObject::rotate(float dxAngle, float dyAngle, float dzAngle) {
    rotationAngles.x += dxAngle;
    rotationAngles.y += dyAngle;
    rotationAngles.z += dzAngle;

    clampAngles();
}

void RigidObject::scale(float scale) {
    this->scaleFactor = scale;
}

void RigidObject::scaleDt(float scale) {
    this->scaleFactor += scale;
    if(this->scaleFactor < 0.1)
        this->scaleFactor = 0.1;
}

const vec3&RigidObject::getRotations() const {
    return this->rotationAngles;
}

const vec3& RigidObject::getPosition() const {
    return this->position;
}

glm::vec3 RigidObject::getClosestPoint(const glm::vec3& point){
    return vec3(0,0,0);
}

const mat4&RigidObject::getModelMatrix() {
    return modelMatrix;
}

void RigidObject::update() {
    this->modelMatrix = constructTranslationMatrix() *
                        constructRotationMatrix() *
                        constructScaleMatrix();
}