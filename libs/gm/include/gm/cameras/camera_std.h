//
// Created by jakub on 2/27/16.
//

#ifndef MG1_CAMERA_STD_H
#define MG1_CAMERA_STD_H

#include <gm/projections/projection.h>
#include "gm/cameras/camera.h"

/*
 * Camera produces View and Projection Matrix.
 *
 * Takes ownership over projection.
 */
class CameraSTD : public Camera{

public:
    CameraSTD(Projection* projection);

    ~CameraSTD();

    const glm::mat4& getVPMatrix();

    virtual const glm::vec3& getDirection() override;
    virtual const glm::mat4 &getViewMatrix();
};


#endif //MG1_CAMERA_STD_H

