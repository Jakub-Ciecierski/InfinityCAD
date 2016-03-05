//
// Created by jakub on 2/28/16.
//

#include <gm/projections/perspective_projection.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

PerspectiveProjection::PerspectiveProjection(float r) :
    r(r){
    projectionMatrix = mat4(1.0f);
    projectionMatrix[2].z = 0;
    projectionMatrix[2].w = 1.0f / r;
}

PerspectiveProjection::~PerspectiveProjection() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

const mat4 &PerspectiveProjection::getProjectionMatrix() {
    return projectionMatrix;
}
