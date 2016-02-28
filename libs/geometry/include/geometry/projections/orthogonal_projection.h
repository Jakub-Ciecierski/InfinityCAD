//
// Created by jakub on 2/28/16.
//

#ifndef MG1_ORTHOGONALPROJECTION_H
#define MG1_ORTHOGONALPROJECTION_H

#include <geometry/projection.h>

class OrthogonalProjection : public Projection {
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

    OrthogonalProjection(float r);

    ~OrthogonalProjection();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual const glm::mat4 &getProjectionMatrix() override;
};


#endif //MG1_ORTHOGONALPROJECTION_H
