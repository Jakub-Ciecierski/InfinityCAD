//
// Created by jakub on 2/28/16.
//

#include <gm/projections/perspective_projection.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

PerspectiveProjection::PerspectiveProjection(float r) :
    projectionDistance(r){
    update();
}

PerspectiveProjection::~PerspectiveProjection() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void PerspectiveProjection::setProjectionDistance(float r) {
    this->projectionDistance = r;
}

void PerspectiveProjection::update() {
    projectionMatrix = mat4(1.0f);

    projectionMatrix[2].z = 0.001;
    projectionMatrix[2].w = 1.0f / projectionDistance;


    float near = 1.0;
    float far = 2.0;

    float top = 1.0f;
    float bottom = -1.0f;

    float left = -1.0f;
    float right = 1.0f;

    float width = right - left;
    float height = top - bottom;
/*
    projectionMatrix[0].x = near / width;
    projectionMatrix[1].y = near / height;

    projectionMatrix[2].z = -(far + near) / (far - near);
    projectionMatrix[2].w = -1;

    projectionMatrix[3].z = -(2*far*near) / (far - near);
    projectionMatrix[3].w = 0;
*/

}
