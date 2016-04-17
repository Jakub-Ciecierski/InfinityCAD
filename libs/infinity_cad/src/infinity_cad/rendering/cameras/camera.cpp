//
// Created by jakub on 3/5/16.
//


#include <infinity_cad/rendering/cameras/camera.h>

Camera::Camera(SceneID id,
               Projection *projection) :
        RigidObject(id),
        projection(projection){

}

Camera::Camera(SceneID id, std::string name,
               Projection *projection) :
        RigidObject(id, name),
        projection(projection){

}

Camera::~Camera() {
    delete projection;
}

Projection *Camera::getProjection() {
    return this->projection;
}