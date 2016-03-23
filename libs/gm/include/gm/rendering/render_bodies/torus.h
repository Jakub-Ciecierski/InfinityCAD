//
// Created by jakub on 2/26/16.
//

#ifndef MG1_TORUS_H
#define MG1_TORUS_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gm/edge.h>
#include <gm/rendering/render_body.h>

class Torus : public RenderBody{
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    float innerRadius;
    float outerRadius;

    unsigned int ringsCount;
    unsigned int sidesCount;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    float getX(float ringAngle, float sideAngle);
    float getY(float ringAngle, float sideAngle);
    float getZ(float ringAngle, float sideAngle);

protected:
    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    virtual void initVertices() override;
    virtual void initEdges() override;

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Torus();

    Torus(float innerRadius, float outerRadiusr);

    Torus(float innerRadius, float outerRadiusr,
          unsigned int sidesCount,
          unsigned int ringsCount);

    ~Torus();

    void updateNetSize(unsigned int ringsCount, unsigned int sidesCount);

    virtual float intersect(const RayCast &ray) override;
};


#endif //MG1_TORUS_H
