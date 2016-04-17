//
// Created by jakub on 2/28/16.
//

#ifndef MG1_ORTHOGONALPROJECTION_H
#define MG1_ORTHOGONALPROJECTION_H

#include "infinity_cad/rendering/projections/projection.h"

class PerspectiveProjection : public Projection {
private:
    unsigned int* widthWindow;
    unsigned int* heightWindow;

protected:

    float projectionDistance;

public:

    PerspectiveProjection(unsigned int* widthWindow, unsigned int* heightWindow,
                          float r);

    ~PerspectiveProjection();

    void setProjectionDistance(float r);

    virtual void update() override;
};


#endif //MG1_ORTHOGONALPROJECTION_H
