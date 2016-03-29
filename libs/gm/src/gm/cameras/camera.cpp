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

glm::vec3 Camera::getClosestPoint(const glm::vec3 point) {
    return glm::vec3(0,0,0);
}
