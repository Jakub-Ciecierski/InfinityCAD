//
// Created by jakub on 3/7/16.
//

#include <gm/projections/indentity_projection.h>

using namespace glm;

IndentityProjection::IndentityProjection() {
    projectionMatrix = mat4(1.0f);
}

IndentityProjection::~IndentityProjection() {

}

void IndentityProjection::update() {

}
