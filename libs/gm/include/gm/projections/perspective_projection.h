//
// Created by jakub on 2/28/16.
//

#ifndef MG1_ORTHOGONALPROJECTION_H
#define MG1_ORTHOGONALPROJECTION_H

#include <gm/projections/projection.h>

class PerspectiveProjection : public Projection {
protected:

    float projectionDistance;

public:

    PerspectiveProjection(float r);

    ~PerspectiveProjection();

    void setProjectionDistance(float r);

    virtual void update() override;
};


#endif //MG1_ORTHOGONALPROJECTION_H
