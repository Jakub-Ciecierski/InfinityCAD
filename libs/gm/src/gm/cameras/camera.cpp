
#include <gm/cameras/camera.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Camera::Camera(Projection* projection) : projection(projection){

}

Camera::~Camera() {
    delete projection;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

const glm::mat4& Camera::getVPMatrix() {
    VP = projection->getProjectionMatrix() * getModelMatrix();
    return VP;
}
