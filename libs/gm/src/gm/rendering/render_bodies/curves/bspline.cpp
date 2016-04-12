//
// Created by jakub on 4/10/16.
//

#include <gm/rendering/render_bodies/curves/bspline.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BSpline::BSpline(SceneID id, std::string name) :
    Spline(id, name){
    // TODO hardcoded degree
    degree = 3;
}

BSpline::~BSpline(){

}

//-----------------------//
//  PROTECTED
//-----------------------//

void BSpline::initVertices(){ }

void BSpline::initEdges() { }


void BSpline::draw(const glm::mat4 &VP, const Color& color) {

}

void BSpline::buildCurve() {
    knotVector.clear();

    int n = this->getControlPointCount();
    int d = this->getDegree();
    int knotCount = n + d;
    for(unsigned int i = 0; i < knotCount; i++){
        if(i < d){
            knotVector.push_back(0);
        } else if(i > n - 1){
            float t = n - d + 1;
            knotVector.push_back(t);
        }else{
            float t = i - d + 1;
            knotVector.push_back(t);
        }
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
    return degree;
}

int BSpline::getControlPointCount(){
    return this->points.size();
}
int BSpline::getKnotCount(){
    return this->knotVector.size();
}

float BSpline::getKnotMin(){
    int size = knotVector.size();
    if(size == 0){
        return -1;
    }
    return knotVector[0];
}

float BSpline::getKnowMax(){
    int size = knotVector.size();
    if(size == 0){
        return -1;
    }
    return knotVector[size - 1];
}

const std::vector<float>& BSpline::getKnotVector(){
    return this->knotVector;
}