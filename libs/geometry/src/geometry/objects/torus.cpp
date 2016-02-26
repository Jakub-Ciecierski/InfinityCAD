//
// Created by jakub on 2/26/16.
//

#include <geometry/objects/torus.h>
#include <math.h>

#define PI 3.14159265

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Torus::Torus(double R, double r) : R(R), r(r){

}

Torus::~Torus() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//


double Torus::getX(double centerCircleAngleDegree,
                   double insideCircleAngleDegree) {
    double x = (R + r*cos(centerCircleAngleDegree * PI/180)) *
            (cos(insideCircleAngleDegree * PI/180));

    return x;
}

double Torus::getY(double centerCircleAngleDegree,
                   double insideCircleAngleDegree) {
    double y = (R + r*cos(centerCircleAngleDegree * PI/180)) *
               (sin(insideCircleAngleDegree * PI/180));

    return y;
}

double Torus::getZ(double centerCircleAngleDegree,
                   double insideCircleAngleDegree) {
    double z = r*cos(centerCircleAngleDegree * PI/180);

    return z;
}
