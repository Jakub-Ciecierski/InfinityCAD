//
// Created by jakub on 2/27/16.
//

#ifndef MG1_CAMERA_H
#define MG1_CAMERA_H

#include <geometry/object.h>

class Camera : public Object{
public:
    Camera();

    ~Camera();

    const glm::mat4& getViewMatrix();
};


#endif //MG1_CAMERA_H

