//
// Created by jakub on 4/11/16.
//

#include <algorithm>
#include <infinity_cad/rendering/render_objects/curves/spline.h>

using namespace std;
using namespace ifc;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Spline::Spline(SceneID sceneID, std::string name) :
    RenderObject(sceneID, name){
    grabable = false;
}

Spline::~Spline(){
    grabable = false;
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


void Spline::addPoint(ifc::Point* point){
    this->points.push_back(point);
    this->components.push_back(point);

    buildCurve();
}

void Spline::removePoint(ifc::Point* point){
    points.erase(remove(points.begin(), points.end(), point),
                 points.end());
    components.erase(remove(components.begin(), components.end(), point),
                     components.end());
    buildCurve();
}

void Spline::swapPoints(int i, int j){
    int pointsCount = points.size();
    if(i < 0 || j < 0 || i >= pointsCount  || j >= pointsCount ) return;

    Point* pointTMP = (ifc::Point*)points[i];
    points[i] = points[j];
    points[j] = pointTMP;

    buildCurve();

}

void Spline::moveUp(ifc::Point* point){
    int i = getPointIndex(point);
    if(i <= 0) return;

    swapPoints(i, i-1);

    buildCurve();
}

void Spline::moveDown(ifc::Point* point){
    int i = getPointIndex(point);
    int pointsCount = points.size();
    if(i < 0 || i == pointsCount - 1) return;

    swapPoints(i, i+1);

    buildCurve();
}

int Spline::getPointIndex(ifc::Point* point){
    for(unsigned int i = 0; i < points.size(); i++){
        if (points[i] == point) return i;
    }
    return -1;
}

const std::vector<ifc::Point*>& Spline::getControlPoints(){
    return points;
}

void Spline::setDrawBezierPolygon(bool value){
    this->doDrawBezierPolygon = value;
}

bool Spline::isDrawBezierPolygon(){
    return this->doDrawBezierPolygon;
}


void Spline::render(const glm::mat4 &VP) {
    draw(VP, this->color);
}

void Spline::render(const glm::mat4 &VP, const Color &color) {
    draw(VP, color);
}
