//
// Created by jakub on 3/11/16.
//


#include <infinity_cad/rendering/projections/stereoscopic_projection.h>
#include <infinity_cad/rendering/color/color_settings.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

StereoscopicProjection::StereoscopicProjection(unsigned int* widthWindow,
                                               unsigned int* heightWindow,
                                               float projectionDistance,
                                               float distance3D) :
        PerspectiveProjection(widthWindow, heightWindow,
                              projectionDistance),
        distance3D(distance3D),
        leftColor(COLOR_STEREOSCOPIC_LEFT_PROJ_DEFAULT),
        rightColor(COLOR_STEREOSCOPIC_RIGHT_PROJ_DEFAULT){
    update();
}

StereoscopicProjection::StereoscopicProjection(unsigned int* widthWindow,
                                               unsigned int* heightWindow,
                                               float projectionDistance,
                                               float distance3D,
                                               Color leftColor,
                                               Color rightColor) :
        PerspectiveProjection(widthWindow, heightWindow, projectionDistance),
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

    leftProjectionMatrix = projectionMatrix;
    rightProjectionMatrix = projectionMatrix;

    leftProjectionMatrix[2].x = -distance3D / 2 * projectionDistance;
    rightProjectionMatrix[2].x = distance3D / 2 * projectionDistance;

/*
    leftProjectionMatrix = mat4(1.0f);
    leftProjectionMatrix[2].x = -distance3D / 2 * projectionDistance;
    leftProjectionMatrix[2].z = 0.001;
    leftProjectionMatrix[2].w = 1.0f / projectionDistance;

    rightProjectionMatrix = mat4(1.0f);
    rightProjectionMatrix[2].x = distance3D / 2 * projectionDistance;
    rightProjectionMatrix[2].z = 0.001;
    rightProjectionMatrix[2].w = 1.0f / projectionDistance;
*/
}