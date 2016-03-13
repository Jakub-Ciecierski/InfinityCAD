//
// Created by jakub on 3/11/16.
//

#include <glm/detail/type_mat4x4.hpp>
#include <gm/projections/stereoscopic_projection.h>
#include <gm/color/color_settings.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

StereoscopicProjection::StereoscopicProjection(float projectionDistance,
                                               float distance3D) :
        PerspectiveProjection(projectionDistance),
        distance3D(distance3D),
        leftColor(COLOR_STEREOSCOPIC_LEFT_PROJ_DEFAULT),
        rightColor(COLOR_STEREOSCOPIC_RIGHT_PROJ_DEFAULT){
    update();
}

StereoscopicProjection::StereoscopicProjection(float projectionDistance,
                                               float distance3D,
                                               Color leftColor,
                                               Color rightColor) :
        PerspectiveProjection(projectionDistance),
        distance3D(distance3D),
        leftColor(leftColor), rightColor(rightColor){
    update();
}


StereoscopicProjection::~StereoscopicProjection() {

}

//-----------------------//
//  PUBLIC
//-----------------------//

void StereoscopicProjection::setDistance3D(float distance3D) {
    this->distance3D = distance3D;
}

void StereoscopicProjection::setLeftColor(Color c) {
    this->leftColor = c;
}

void StereoscopicProjection::setRightColor(Color c) {
    this->rightColor = c;
}


const mat4& StereoscopicProjection::getLeftProjectionMatrix() const{
    return leftProjectionMatrix;
}

const mat4& StereoscopicProjection::getRightProjectionMatrix() const {
    return rightProjectionMatrix;
}

const Color& StereoscopicProjection::getLeftColor() const {
    return leftColor;
}

const Color& StereoscopicProjection::getRightColor() const {
    return rightColor;
}


void StereoscopicProjection::update() {
    PerspectiveProjection::update();

    leftProjectionMatrix = mat4(1.0f);
    leftProjectionMatrix[2].x = -distance3D / 2 * projectionDistance;
    leftProjectionMatrix[2].z = 0;
    leftProjectionMatrix[2].w = 1.0f / projectionDistance;

    rightProjectionMatrix = mat4(1.0f);
    rightProjectionMatrix[2].x = distance3D / 2 * projectionDistance;
    rightProjectionMatrix[2].z = 0;
    rightProjectionMatrix[2].w = 1.0f / projectionDistance;
}