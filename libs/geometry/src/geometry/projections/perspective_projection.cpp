//
// Created by jakub on 2/28/16.
//

#include <drawer.h>
#include <geometry/projections/perspective_projection.h>

using namespace glm;

PerspectiveProjection::PerspectiveProjection(int width, int height) {
    this->width = width;
    this->height = height;

    FOV = 90;
    projectionMatrix = mat4(1.0f);
}

PerspectiveProjection::~PerspectiveProjection() {

}

const glm::mat4 &PerspectiveProjection::getProjectionMatrix() {
    float d = (width / 2) * (1 / tan(angleToRadians(FOV/2)));
    float cx = width / 2;
    float cy = height / 2;

    projectionMatrix[0].x = d;
    projectionMatrix[0].z = cx;

    projectionMatrix[1].y = -d;
    projectionMatrix[1].z = cy;

    projectionMatrix[2].z = 0;
    projectionMatrix[2].w = 1;

    projectionMatrix[3].z = 1;
    projectionMatrix[3].w = 0;

    return projectionMatrix;
}