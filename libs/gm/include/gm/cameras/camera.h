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

    Camera();
    Camera(Projection* projection);

    virtual ~Camera();

    Projection* getProjection();
    virtual const glm::mat4& getVPMatrix() = 0;
    virtual const glm::mat4& getViewMatrix() = 0;
};


#endif //MG1_CAMERA_H
