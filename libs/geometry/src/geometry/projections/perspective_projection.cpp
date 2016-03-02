//
// Created by jakub on 2/28/16.
//

#include <drawer.h>
#include <geometry/projections/perspective_projection.h>

using namespace glm;

PerspectiveProjection::PerspectiveProjection(int width, int height) {
    this->width = width;
    this->height = height;

    FOV = 130;
    projectionMatrix = mat4(1.0f);

    float d = (width / 2) * (1 / tan(angleToRadians(FOV/2)));
    float cx = width / 2;
    float cy = height / 2;

    projectionMatrix[0].x = d;
    projectionMatrix[0].y = 0;
    projectionMatrix[0].z = 0;
    projectionMatrix[0].w = 0;

    projectionMatrix[1].x = 0;
    projectionMatrix[1].y = -d;
    projectionMatrix[1].z = 0;
    projectionMatrix[1].w = 0;

    projectionMatrix[2].x = cx;
    projectionMatrix[2].y = cy;
    projectionMatrix[2].z = 0;
    projectionMatrix[2].w = 1;

    projectionMatrix[3].x = 0;
    projectionMatrix[3].y = 0;
    projectionMatrix[3].z = 1;
    projectionMatrix[3].w = 0;
}

PerspectiveProjection::~PerspectiveProjection() {

}

const glm::mat4 &PerspectiveProjection::getProjectionMatrix() {
    return projectionMatrix;
}