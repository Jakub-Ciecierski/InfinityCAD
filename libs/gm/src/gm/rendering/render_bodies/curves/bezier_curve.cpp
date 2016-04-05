//
// Created by jakub on 4/5/16.
//

#include <gm/rendering/render_bodies/curves/bezier_curve.h>
#include <algorithm>

using namespace ic;
using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BezierCurve::BezierCurve(SceneID id, std::string name) :
        RenderBody(id, name){

}

BezierCurve::~BezierCurve(){

}


//-----------------------//
//  PROTECTED
//-----------------------//

void BezierCurve::initVertices(){

}

void BezierCurve::initEdges(){

}

//-----------------------//
//  PUBLIC
//-----------------------//


void BezierCurve::addPoint(ic::Point* point){
    this->points.push_back(point);
}

void BezierCurve::removePoint(ic::Point* point){
    points.erase(remove(points.begin(), points.end(), point),
                 points.end());
}

void BezierCurve::swapPoints(int i, int j){
    if(i < 0 || j < 0 || i >= points.size() || j >= points.size()) return;

    Point* pointTMP = points[i];
    points[i] = points[j];
    points[j] = pointTMP;

}

void BezierCurve::moveUp(ic::Point* point){
    int i = getPointIndex(point);
    if(i <= 0) return;

    swapPoints(i, i-1);
}
void BezierCurve::moveDown(ic::Point* point){
    int i = getPointIndex(point);
    if(i < 0 || i == points.size() - 1) return;

    swapPoints(i, i+1);
}

int BezierCurve::getPointIndex(ic::Point* point){
    for(unsigned int i = 0; i < points.size(); i++){
        if (points[i] == point) return i;
    }
    return -1;
}

const vector<ic::Point*>& BezierCurve::getPoints(){
    return this->points;
}

float BezierCurve::intersect(const RayCast &ray){
    return RAYCAST_NO_SOLUTION;
}
glm::vec3 BezierCurve::getClosestPoint(const glm::vec3 point){
    return vec3(0,0,0);
}
