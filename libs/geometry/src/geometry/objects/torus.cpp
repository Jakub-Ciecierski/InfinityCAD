//
// Created by jakub on 2/26/16.
//

#include <geometry/objects/torus.h>
#include <cmath>
#include <GL/gl.h>
#include <iostream>
#include <drawer.h>

using namespace glm;

const double PI_OVER_180 = M_PI/180;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Torus::Torus(float innerRadius, float outerRadiusr) :
        innerRadius(innerRadius), outerRadiusr(outerRadiusr){
    sidesCount = 90;
    ringsCount = 90;
    initVertices();
    initEdges();
}

Torus::Torus(float innerRadius, float outerRadiusr,
             unsigned int sidesCount,
             unsigned int ringsCount) :
        innerRadius(innerRadius), outerRadiusr(outerRadiusr),
        sidesCount(sidesCount), ringsCount(ringsCount){
    initVertices();
    initEdges();
}

Torus::~Torus() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//


float Torus::getX(float centerCircleAngleDegree,
                  float insideCircleAngleDegree) {
    float x = (innerRadius + outerRadiusr * cos(angleToRadians(centerCircleAngleDegree))) *
              (cos(angleToRadians(insideCircleAngleDegree)));

    return x;
}

float Torus::getY(float centerCircleAngleDegree,
                  float insideCircleAngleDegree) {
    float y = (innerRadius + outerRadiusr * cos(angleToRadians(centerCircleAngleDegree))) *
              (sin(angleToRadians(insideCircleAngleDegree)));

    return y;
}

float Torus::getZ(float centerCircleAngleDegree,
                  float insideCircleAngleDegree) {
    float z = outerRadiusr * cos(angleToRadians(centerCircleAngleDegree));

    return z;
}

//-----------------------------------------------------------//
//  PROTECTED METHODS
//-----------------------------------------------------------//

void Torus::initVertices() {
    int vertexCount = sidesCount * ringsCount;
    vertices.clear();
    vertices.resize(vertexCount);

    int currentVertexIndex = 0;
    const unsigned int MAX_ANGLE = 360;
    for(unsigned int j = 0; j < ringsCount; j++){
        float ringIndex = ((float)j / ringsCount) * MAX_ANGLE;
        for(unsigned int i = 0; i < sidesCount; i++){
            float sideIndex = ((float)i / sidesCount) * MAX_ANGLE;

            float x = this->getX(ringIndex,
                                 sideIndex);
            float y = this->getY(ringIndex,
                                 sideIndex);
            float z = this->getZ(ringIndex,
                                 sideIndex);

            fvec4 vertex = fvec4(x, y, z, 1.0f);
            vertices[currentVertexIndex++] = vertex;
        }
    }
}


void Torus::initEdges() {
    for(unsigned int r = 0; r < ringsCount - 1; r++){
        for(unsigned int s = 0; s < sidesCount - 1; s++){
            Edge edge1(r * sidesCount + s, r * sidesCount + (s + 1));
            Edge edge2((r +1) * sidesCount + (s + 1), (r +1) * sidesCount + s);
            Edge edge3(r * sidesCount + (s + 1), (r +1) * sidesCount + (s + 1));
            edges.push_back(edge1);
            edges.push_back(edge2);
            edges.push_back(edge3);
        }
    }
}