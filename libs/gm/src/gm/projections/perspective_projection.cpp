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
    projectionMatrix[2].z = 0;
    projectionMatrix[2].w = 1.0f / projectionDistance;
}
