//
// Created by jakub on 2/26/16.
//

#include <geometry/objects/torus.h>
#include <cmath>
#include <GL/gl.h>
#include <iostream>

using namespace glm;

const double PI_OVER_180 = M_PI/180;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Torus::Torus(double R, double r) : R(R), r(r){
    centerPointCount = 360;
    insidePointCount = 360;

    initVertices();

}

Torus::~Torus() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//


double Torus::getX(double centerCircleAngleDegree,
                   double insideCircleAngleDegree) {
    double x = (R + r*cos(angleToRadians(centerCircleAngleDegree))) *
            (cos(angleToRadians(insideCircleAngleDegree)));

    return x;
}

double Torus::getY(double centerCircleAngleDegree,
                   double insideCircleAngleDegree) {
    double y = (R + r*cos(angleToRadians(centerCircleAngleDegree))) *
               (sin(angleToRadians(insideCircleAngleDegree)));

    return y;
}

double Torus::getZ(double centerCircleAngleDegree,
                   double insideCircleAngleDegree) {
    double z = r*cos(angleToRadians(centerCircleAngleDegree));

    return z;
}

void Torus::update() {

}

void Torus::initVertices() {
    int vertexCount = centerPointCount * insidePointCount;
    vertices.clear();
    vertices.resize(vertexCount);

    int currentVertexIndex = 0;
    const unsigned int MAX_ANGLE = 360;
    for(unsigned int i = 0; i < centerPointCount; i++){
        double centerCircleAngleDegree =
                ((double)i/centerPointCount) * MAX_ANGLE;
        for(unsigned int j = 0; j < insidePointCount; j++){
            double insideCircleAngleDegree =
                    ((double)j/insidePointCount) * MAX_ANGLE;

            double X = this->getX(centerCircleAngleDegree,
                                  insideCircleAngleDegree);
            double Y = this->getY(centerCircleAngleDegree,
                                  insideCircleAngleDegree);
            double Z = this->getZ(centerCircleAngleDegree,
                                  insideCircleAngleDegree);

            fvec4 vertex = fvec4(X, Y, Z, 1.0f);
            vertices[currentVertexIndex++] = vertex;
        }
    }
}

void Torus::render(const mat4& MVP) {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    for(int i = 0; i < vertices.size() - 1; i++){
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        glVertex3f(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
    }

    glEnd();
}

double Torus::angleToRadians(double angle) {
    double radians = angle * PI_OVER_180;
    return radians;
}
