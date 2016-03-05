//
// Created by jakub on 2/26/16.
//

#include <gm/objects/torus.h>

using namespace glm;
using namespace std;

const double PI_OVER_180 = M_PI/180;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Torus::Torus(float innerRadius, float outerRadiusr) :
        innerRadius(innerRadius), outerRadius(outerRadiusr){
    sidesCount = 30;
    ringsCount = 30;
    initVertices();
    initEdges();
}

Torus::Torus(float innerRadius, float outerRadiusr,
             unsigned int ringsCount,
             unsigned int sidesCount) :
        innerRadius(innerRadius), outerRadius(outerRadiusr),
        ringsCount(ringsCount), sidesCount(sidesCount){
    initVertices();
    initEdges();
}

Torus::~Torus() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//


float Torus::getX(float ringAngle,
                  float sideAngle) {
    float x = (innerRadius + outerRadius * cos(ringAngle))
              * (cos((sideAngle)));
    return x;
}

float Torus::getY(float ringAngle,
                  float sideAngle) {
    float y = (innerRadius + outerRadius * cos((ringAngle)))
              * (sin((sideAngle)));

    return y;
}

float Torus::getZ(float ringAngle,
                  float sideAngle) {
    float z = outerRadius * sin(ringAngle);

    return z;
}

//-----------------------------------------------------------//
//  PROTECTED METHODS
//-----------------------------------------------------------//

void Torus::initVertices() {
    vertices.clear();
    edges.clear();

    unsigned int currentVertexIndex = 0;

    float rd = (2 * M_PI) / ringsCount;
    float sd = (2 * M_PI) / sidesCount;
    float currentRAngle = 0;
    float currentSAngle = 0;

    while(currentRAngle < 2*M_PI){
        currentSAngle = 0;
        while(currentSAngle < 2*M_PI){
            fvec4 vertex = fvec4(this->getX(currentRAngle, currentSAngle),
                                 this->getY(currentRAngle, currentSAngle),
                                 this->getZ(currentRAngle, currentSAngle),
                                 1.0f);
            fvec4 vertex1 = fvec4(this->getX(currentRAngle + rd, currentSAngle),
                                  this->getY(currentRAngle + rd, currentSAngle),
                                  this->getZ(currentRAngle + rd, currentSAngle),
                                  1.0f);
            fvec4 vertex2 = fvec4(this->getX(currentRAngle + rd,
                                             currentSAngle + sd),
                                  this->getY(currentRAngle + rd,
                                             currentSAngle + sd),
                                  this->getZ(currentRAngle + rd,
                                             currentSAngle + sd),
                                  1.0f);
            fvec4 vertex3 = fvec4(this->getX(currentRAngle,
                                             currentSAngle + sd),
                                  this->getY(currentRAngle,
                                             currentSAngle + sd),
                                  this->getZ(currentRAngle,
                                             currentSAngle + sd),
                                  1.0f);
            vertices.push_back(vertex);
            vertices.push_back(vertex1);
            vertices.push_back(vertex2);
            vertices.push_back(vertex3);

            edges.push_back(Edge(currentVertexIndex, currentVertexIndex+1));
            edges.push_back(Edge(currentVertexIndex + 1,
                                 currentVertexIndex + 2));
            edges.push_back(Edge(currentVertexIndex + 2,
                                 currentVertexIndex + 3));
            edges.push_back(Edge(currentVertexIndex + 3,
                                 currentVertexIndex));
            currentVertexIndex += 4;
            currentSAngle += sd;
        }
        currentRAngle += rd;
    }
}


void Torus::initEdges() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void Torus::updateNetSize(unsigned int ringsCount,
                          unsigned int sidesCount){
    this->ringsCount = ringsCount;
    this->sidesCount = sidesCount;

    initVertices();
}