//
// Created by jakub on 3/7/16.
//

#ifndef MG1_INDENTITY_PROJECTION_H
#define MG1_INDENTITY_PROJECTION_H

#include "gm/projections/projection.h"

class IndentityProjection : public Projection{
public:

    IndentityProjection();

    ~IndentityProjection();

    virtual void update() override;
};


#endif //MG1_INDENTITY_PROJECTION_H
