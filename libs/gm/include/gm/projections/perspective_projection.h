//
// Created by jakub on 2/28/16.
//

#ifndef MG1_ORTHOGONALPROJECTION_H
#define MG1_ORTHOGONALPROJECTION_H

#include <gm/projection.h>

class PerspectiveProjection : public Projection {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    float r;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    PerspectiveProjection(float r);

    ~PerspectiveProjection();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual const glm::mat4 &getProjectionMatrix() override;
};


#endif //MG1_ORTHOGONALPROJECTION_H
