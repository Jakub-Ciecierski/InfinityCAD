//
// Created by jakub on 2/26/16.
//

#ifndef MG1_TORUS_H
#define MG1_TORUS_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Torus {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    double R;
    double r;

    unsigned int centerPointCount;
    unsigned int insidePointCount;

    std::vector<glm::fvec4> verticies;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    void initVertices();
public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Torus(double R, double r);

    ~Torus();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void update();

    double getX(double centerCircleAngleDegree, double insideCircleAngleDegree);
    double getY(double centerCircleAngleDegree, double insideCircleAngleDegree);
    double getZ(double centerCircleAngleDegree, double insideCircleAngleDegree);
};


#endif //MG1_TORUS_H
