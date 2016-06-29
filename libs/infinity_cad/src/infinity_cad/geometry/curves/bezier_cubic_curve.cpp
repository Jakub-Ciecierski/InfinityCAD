//
// Created by jakub on 4/6/16.
//


#include <stdexcept>
#include <infinity_cad/geometry/curves/bezier_cubic_curve.h>
#include <infinity_cad/geometry/polynomials/bernstein_basis.h>
#include <infinity_cad/math/math.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BezierCubicCurve::BezierCubicCurve(){
    p0 = p1 = p2 = p3 = NULL;

    points.resize(4);
    points[0] = &p0;
    points[1] = &p1;
    points[2] = &p2;
    points[3] = &p3;
}

BezierCubicCurve::BezierCubicCurve(ifc::Point* P0, ifc::Point* P1,
                                   ifc::Point* P2, ifc::Point* P3){
    p0 = P0;
    p1 = P1;
    p2 = P2;
    p3 = P3;


    points.resize(4);
    points[0] = &p0;
    points[1] = &p1;
    points[2] = &p2;
    points[3] = &p3;
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
    }else{
        throw new std::invalid_argument(
                "Degree: " + std::to_string(deg) + " not supported");
    }
}


void BezierCubicCurve::addPoint(ifc::Point* p){
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

ifc::Point* BezierCubicCurve::getPoint(int i){
    if(i == 0)
        return p0;
    else if(i == 1)
        return p1;
    else if(i == 2)
        return p2;
    else if(i == 3)
        return p3;
    else
        return NULL;
}

float BezierCubicCurve::getAverageDistanceBetweenPoints(){
    int deg = degree();

    float sumOfDistinctDinstances = 0;
    int distancesCount = 0;
    for(int i = 0; i < deg+1-1; i++){
        for(int j = i+1; j < deg+1; j++){
            const ifc::Point* p1 = getPoint(i);
            const ifc::Point* p2 = getPoint(j);

            sumOfDistinctDinstances +=
                    ifc::euclideanDistance(p1->getPosition(),
                                           p2->getPosition());

            distancesCount++;
        }
    }
    return (sumOfDistinctDinstances / distancesCount);
}

float BezierCubicCurve::getMaximumDistanceBetweenPoints(){
    int deg = degree();

    float maxDistance = -1;
    for(int i = 0; i < deg+1-1; i++){
        for(int j = i+1; j < deg+1; j++){
            const ifc::Point* p1 = getPoint(i);
            const ifc::Point* p2 = getPoint(j);

            float dist = ifc::euclideanDistance(p1->getPosition(),
                                                p2->getPosition());

            if(maxDistance < dist){
                maxDistance = dist;
            }
        }
    }
    return maxDistance;
}

int BezierCubicCurve::degree(){
    if(p0 == NULL) return -1;       // 0 points
    else if(p1 == NULL) return 0;   // 1 point
    else if(p2 == NULL) return 1;   // 2 points
    else if(p3 == NULL) return 2;   // 3 points
    else return 3;                  // 4 points
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
