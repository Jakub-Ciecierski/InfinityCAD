//
// Created by jakub on 3/5/16.
//

#ifndef MG1_CAMERA_H
#define MG1_CAMERA_H


#include <gm/object.h>
#include <gm/projections/projection.h>

class Camera : public Object{
protected:
    Projection* projection;

    glm::mat4 VP;

public:

    Camera(Projection* projection);

    virtual ~Camera();

    virtual const glm::mat4& getVPMatrix() = 0;
};


#endif //MG1_CAMERA_H
