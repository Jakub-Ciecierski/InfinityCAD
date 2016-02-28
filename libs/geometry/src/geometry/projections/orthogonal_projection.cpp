//
// Created by jakub on 2/28/16.
//

#include <geometry/projections/orthogonal_projection.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

OrthogonalProjection::OrthogonalProjection(float r) :
    r(r){
}

OrthogonalProjection::~OrthogonalProjection() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

const mat4 &OrthogonalProjection::getProjectionMatrix() {
    projectionMatrix = mat4(1.0f);
    projectionMatrix[3].z = 1.0f / r;

    return projectionMatrix;
}
