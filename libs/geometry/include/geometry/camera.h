//
// Created by jakub on 2/27/16.
//

#ifndef MG1_CAMERA_H
#define MG1_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    unsigned int x;
    unsigned int y;
    unsigned int z;

    float xAngle;
    float yAngle;
    float zAngle;

    glm::mat4 MVP;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Camera(unsigned int x, unsigned int y, unsigned int z);

    ~Camera();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void updateMVP();

    const glm::mat4& getMVP();
};


#endif //MG1_CAMERA_H
