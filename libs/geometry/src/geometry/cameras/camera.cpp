
#include <geometry/cameras/camera.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Camera::Camera(){

}

Camera::~Camera() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

const glm::mat4 &Camera::getViewMatrix() {
    return getModelMatrix();
}
