//
// Created by jakub on 3/7/16.
//

#ifndef MG1_INDENTITY_PROJECTION_H
#define MG1_INDENTITY_PROJECTION_H

#include "gm/projections/projection.h"

class IndentityProjection : public Projection{
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    IndentityProjection();

    ~IndentityProjection();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//
    virtual const glm::mat4 &getProjectionMatrix() override;
};


#endif //MG1_INDENTITY_PROJECTION_H
