//
// Created by jakub on 2/28/16.
//

#ifndef MG1_PROJECTION_H
#define MG1_PROJECTION_H

#include <glm/gtc/matrix_transform.hpp>

class Projection {
protected:
    glm::mat4 projectionMatrix;

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Projection();

    virtual ~Projection();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual const glm::mat4& getProjectionMatrix();

    virtual void update() = 0;
};


#endif //MG1_PROJECTION_H
