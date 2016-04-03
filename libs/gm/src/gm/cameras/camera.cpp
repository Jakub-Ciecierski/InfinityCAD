//
// Created by jakub on 3/5/16.
//


#include <gm/cameras/camera.h>

Camera::Camera(SceneID id,
               Projection *projection) :
        RigidBody(id),
        projection(projection){

}

Camera::Camera(SceneID id, std::string name,
               Projection *projection) :
        RigidBody(id, name),
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
