//
// Created by jakub on 3/5/16.
//


#include <gm/cameras/camera.h>

Camera::Camera(){

}

Camera::Camera(Projection *projection) :
        projection(projection){

}

Camera::~Camera() {
    delete projection;
}

Projection *Camera::getProjection() {
    return this->projection;
}
