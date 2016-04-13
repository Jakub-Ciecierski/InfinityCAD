//
// Created by jakub on 4/10/16.
//

#include <gm/rendering/render_bodies/curves/bspline.h>
#include <gm/polynomials/bspline_basis.h>
#include <iostream>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BSpline::BSpline(SceneID id, std::string name) :
    Spline(id, name){
}

BSpline::~BSpline(){

}

//-----------------------//
//  PRIVATE
//-----------------------//

int BSpline::computeDegree(){
    int pointCount = getControlPointCount();
    if(pointCount == 0)
        return -1;
    else if(pointCount < MAX_DEGREE)
        return pointCount - 1;
    else
        return MAX_DEGREE;
}

//-----------------------//
//  PROTECTED
//-----------------------//

void BSpline::initVertices(){ }

void BSpline::initEdges() { }


void BSpline::draw(const glm::mat4 &VP, const Color& color) {
    int degree = computeDegree();
    if(degree == -1) return;

    float dt = 0.001;
    float t_max = getKnotMax();
    float t = getKnotMin(degree);

    setSurfaceColor(color);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    while(t < t_max){
        vec4 point = computeBSpline(points, knotVector, t, degree);
        t += dt;
        /*
        if(point.x == 0 && point.y == 0)
            std::cout << std::endl;*/
        point = VP * point;
        point.x /= point.w;
        point.y /= point.w;

        if(point.w < 0) continue; // clip
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void BSpline::buildCurve() {
    knotVector.clear();

    int n = this->getControlPointCount();
    int d = this->getDegree();
    int knotCount = n + d + 1;

    for(int i = 0; i < knotCount; i++){
        knotVector.push_back(i);
        /*
        if(i < d + 1){
            knotVector.push_back(0);
        } else if(i > n - 1){
            float t = n - d + 1;
            knotVector.push_back(t);
        }else{
            float t = i - d;
            knotVector.push_back(t);
        }
         */
    }

}

//-----------------------//
//  PUBLIC
//-----------------------//

float BSpline::intersect(const RayCast &ray) {
    return RAYCAST_NO_SOLUTION;
}

glm::vec3 BSpline::getClosestPoint(const glm::vec3 point) {
    return vec3();
}

int BSpline::getDegree(){
    return computeDegree();
}

int BSpline::getControlPointCount(){
    return this->points.size();
}
int BSpline::getKnotCount(){
    return this->knotVector.size();
}

float BSpline::getKnotMin(int degree){
    int size = knotVector.size();
    int index = computeDegree();
    if(index >= size){
        return -1;
    }
    return knotVector[index];
    //return knotVector[0];
}

float BSpline::getKnotMax(){
    int size = knotVector.size();
    int index = getControlPointCount();
    if(index >= size){
        return -1;
    }
    return knotVector[index];
    //return knotVector[size - 1];
}

const std::vector<float>& BSpline::getKnotVector(){
    return this->knotVector;
}
