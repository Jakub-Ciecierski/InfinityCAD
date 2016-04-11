//
// Created by jakub on 4/11/16.
//

#include <gm/rendering/render_bodies/curves/spline.h>
#include <algorithm>
using namespace std;
using namespace ic;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Spline::Spline(SceneID sceneID, std::string name) :
    RenderBody(sceneID, name){

}

Spline::~Spline(){

}


//-----------------------//
//  PROTECTED
//-----------------------//

void Spline::initVertices(){

}

void Spline::initEdges(){

}


//-----------------------//
//  PUBLIC
//-----------------------//


void Spline::addPoint(ic::Point* point){
    this->points.push_back(point);

    buildCurve();
}

void Spline::removePoint(ic::Point* point){
    points.erase(remove(points.begin(), points.end(), point),
                 points.end());

    buildCurve();
}

void Spline::swapPoints(int i, int j){
    if(i < 0 || j < 0 || i >= points.size() || j >= points.size()) return;

    Point* pointTMP = points[i];
    points[i] = points[j];
    points[j] = pointTMP;

    buildCurve();

}

void Spline::moveUp(ic::Point* point){
    int i = getPointIndex(point);
    if(i <= 0) return;

    swapPoints(i, i-1);

    buildCurve();
}

void Spline::moveDown(ic::Point* point){
    int i = getPointIndex(point);
    if(i < 0 || i == points.size() - 1) return;

    swapPoints(i, i+1);

    buildCurve();
}

int Spline::getPointIndex(ic::Point* point){
    for(unsigned int i = 0; i < points.size(); i++){
        if (points[i] == point) return i;
    }
    return -1;
}

const vector<ic::Point*>& Spline::getPoints(){
    return this->points;
}

void Spline::setDrawBezierPolygon(bool value){
    this->doDrawBezierPolygon = value;
}

bool Spline::isDrawBezierPolygon(){
    return this->doDrawBezierPolygon;
}

float Spline::intersect(const RayCast &ray){
    return RAYCAST_NO_SOLUTION;
}

glm::vec3 Spline::getClosestPoint(const glm::vec3 point){
    return vec3(0,0,0);
}

void Spline::render(const glm::mat4 &VP) {
    draw(VP, this->color);
}

void Spline::render(const glm::mat4 &VP, const Color &color) {
    draw(VP, color);
}
