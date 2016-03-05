//
// Created by jakub on 2/27/16.
//

#ifndef MG1_CAMERA_H
#define MG1_CAMERA_H

#include <gm/object.h>
#include <gm/projection.h>

/*
 * Camera produces View and Projection Matrix.
 *
 * Takes ownership over projection.
 */
class Camera : public Object{
private:
    Projection* projection;

    glm::mat4 VP;

public:
    Camera(Projection* projection);

    ~Camera();

    const glm::mat4& getVPMatrix();
};


#endif //MG1_CAMERA_H

