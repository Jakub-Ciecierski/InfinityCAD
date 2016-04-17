//
// Created by jakub on 2/28/16.
//

#include <infinity_cad/rendering/projections/projection.h>

Projection::Projection() {

}

Projection::~Projection() {

}

const glm::mat4 &Projection::getProjectionMatrix() {
    return this->projectionMatrix;
}
