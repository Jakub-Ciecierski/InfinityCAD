//
// Created by jakub on 2/28/16.
//

#ifndef MG1_PERSPECTIVE_PROJECTION_H
#define MG1_PERSPECTIVE_PROJECTION_H

#include <glm/gtc/matrix_transform.hpp>
#include <geometry/projection.h>

class PerspectiveProjection : public Projection{
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    int width;
    int height;

    float FOV;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    PerspectiveProjection(int width, int height);

    ~PerspectiveProjection();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual const glm::mat4 &getProjectionMatrix() override;

};


#endif //MG1_PERSPECTIVE_PROJECTION_H
