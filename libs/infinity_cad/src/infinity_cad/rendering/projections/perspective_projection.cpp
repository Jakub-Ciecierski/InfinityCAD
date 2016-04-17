//
// Created by jakub on 2/28/16.
//

#include <infinity_cad/rendering/projections/perspective_projection.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

PerspectiveProjection::PerspectiveProjection(unsigned int* widthWindow,
                                             unsigned int* heightWindow,
                                             float r) :
        widthWindow(widthWindow), heightWindow(heightWindow),
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
/*
    projectionMatrix = mat4(1.0f);

    projectionMatrix[2].z = 0.001;
    projectionMatrix[2].w = 1.0f / projectionDistance;

*/
    float FoV = 45;
    float aspectRatio = (float) (*widthWindow) / (float) (*heightWindow);

    projectionMatrix = glm::perspective(
            FoV, aspectRatio , 0.1f, 100.0f
    );
    projectionMatrix[2].w = -projectionMatrix[2].w;


}

