//
// Created by jakub on 4/6/16.
//

#include <gm/curves/bezier_cubic_curve.h>

using namespace glm;
using namespace std;

#include <gm/polynomials/bernstein.h>
#include <stdexcept>

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BezierCubicCurve::BezierCubicCurve(){
    p0 = p1 = p2 = p3 = NULL;
}

//-----------------------//
//  PUBLIC
//-----------------------//

vec4 BezierCubicCurve::compute(float t){
    int deg = degree();
    if(deg == -1){
        throw new std::invalid_argument("Cant compute negative degree Bezier");
    }
    if(deg == 0){
        const vec3& pos = p0->getPosition();
        return vec4(pos.x, pos.y, pos.z, 1);
    }
    else if(deg == 1){
        return (linearBernstein(p0->getPosition(),
                                p1->getPosition(), t));
    }else if(deg == 2){
        return (quadraticBernstein(p0->getPosition(), p1->getPosition(),
                                   p2->getPosition(), t));
    }else if(deg == 3){
        return (cubicBernstein(p0->getPosition(), p1->getPosition(),
                               p2->getPosition(), p3->getPosition(), t));
    }
}


void BezierCubicCurve::addPoint(ic::Point* p){
    if(isFull()) return;
    if(p0 == NULL)
        p0 = p;
    else if(p1 == NULL)
        p1 = p;
    else if(p2 == NULL)
        p2 = p;
    else if(p3 == NULL)
        p3 = p;
}

int BezierCubicCurve::degree(){
    if(p0 == NULL) return -1;
    else if(p1 == NULL) return 0;
    else if(p2 == NULL) return 1;
    else if(p3 == NULL) return 2;
    else return 3;
}

bool BezierCubicCurve::isEmpty(){
    return (degree() == -1);
}
bool BezierCubicCurve::isFull(){
    return (MAX_DEGREE_CUBIC == degree());
}

//-----------------------//
//  PRIVATE 
//-----------------------//
