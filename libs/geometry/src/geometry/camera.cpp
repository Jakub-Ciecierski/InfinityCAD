#include <geometry/camera.h>

using namespace glm;

Camera::Camera(unsigned int x, unsigned int y, unsigned int z) :
    x(x), y(y), z(z){
    xAngle = 0;
    yAngle = 0;
    zAngle = 0;

    MVP = mat4(1.0f);
}

Camera::~Camera() {

}

void Camera::updateMVP() {
    mat4 translateMatrix = mat4(1.0f);
    translateMatrix[0].w = x;
    translateMatrix[1].w = y;
    translateMatrix[2].w = z;
}

const glm::mat4 &Camera::getMVP() {
    return MVP;
}
