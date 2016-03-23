#include <gm/cameras/camera_std.h>

using namespace glm;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

CameraSTD::CameraSTD(Projection* projection) :
        Camera(projection){

}

CameraSTD::~CameraSTD() {
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

const mat4 &CameraSTD::getViewMatrix() {
    return getModelMatrix();
}

const mat4& CameraSTD::getVPMatrix() {
    VP = projection->getProjectionMatrix() * getModelMatrix();
    return VP;
}

const vec3 &CameraSTD::getDirection() {
    return vec3(0,0,0);
}
