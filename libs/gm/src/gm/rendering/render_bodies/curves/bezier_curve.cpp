//
// Created by jakub on 4/5/16.
//

#include <gm/rendering/render_bodies/curves/bezier_curve.h>
#include <algorithm>
#include <iostream>

using namespace ic;
using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BezierCurve::BezierCurve(SceneID id, std::string name) :
        RenderBody(id, name){
    this->color = Color(0.8, 0, 0);
}

BezierCurve::BezierCurve(SceneID id, std::string name,
        std::vector<ic::Point*>& points) :
        RenderBody(id, name){
    for(unsigned int i = 0;i < points.size(); i++){
        this->points.push_back(points[i]);
    }

    buildBezierCurves_C0();
}

BezierCurve::~BezierCurve(){

}


//-----------------------//
//  PRIVATE
//-----------------------//


void BezierCurve::buildBezierCurves_C0(){
    std::cout << "Building Bezier Curves C0" << std::endl;

    unsigned int pointsCount = points.size();
    if(pointsCount == 0) return;

    int currentPointIndex = 0;
    int currentSegmentIndex = 0;

    bezierCurves.clear();

    BezierCubicCurve bezierCubicCurve;
    bezierCubicCurve.addPoint(points[0]);
    currentPointIndex++;
    bezierCurves.push_back(bezierCubicCurve);

    bool addNewCurve = false;
    while(currentPointIndex < pointsCount){
        std::cout << "Point Index: " << currentPointIndex << std::endl;
        std::cout << "Curve Index: " << currentSegmentIndex << std::endl;

        if(addNewCurve){
            currentSegmentIndex++;
            bezierCurves.push_back(BezierCubicCurve());
        }
        /*
        BezierCubicCurve& currentCubicCurve =
                bezierCurves[currentSegmentIndex];
        if ((currentPointIndex %
                    BezierCubicCurve::MAX_POINT_COUNT_CUBIC == 0)
                && !currentCubicCurve.isFull()) {
            currentSegmentIndex++;
            bezierCurves.push_back(BezierCubicCurve());
        }*/

        BezierCubicCurve& cubicCurve = bezierCurves[currentSegmentIndex];

        // C0 continuity
        if(cubicCurve.isEmpty()){
            currentPointIndex -= 1;
        }
        cubicCurve.addPoint(points[currentPointIndex]);

        if(cubicCurve.isFull()){
            addNewCurve = true;
        }else{
            addNewCurve = false;
        }
        currentPointIndex++;
    }
    std::cout << "Finished Building Bezier Curves C0" << std::endl;
}

void BezierCurve::printStatusDEBUG() {
    int curvesCount = this->bezierCurves.size();
    std::cout << "---------------------" << std::endl;
    std::cout << "Segment Count: " << curvesCount << std::endl;
    if (curvesCount > 0) {
        int deg = bezierCurves[curvesCount - 1].degree();
        std::cout << "Degree of Last: " << deg  << std::endl;
    }
    std::cout << "---------------------" << std::endl << std::endl;
}

//-----------------------//
//  PROTECTED
//-----------------------//

void BezierCurve::initVertices(){

}

void BezierCurve::initEdges(){

}


void BezierCurve::drawCurves(const glm::mat4 &VP) {
    unsigned int bezierCurvesCount = bezierCurves.size();

    int PIXELS_COUNT = 10000;
    float dt = 1/(float)PIXELS_COUNT;

    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for(unsigned int i = 0;i < bezierCurvesCount; i++){
        BezierCubicCurve& bezierCurveCubic = bezierCurves[i];

        float t = 0;
        while(t <= 1){
            vec4 point = bezierCurveCubic.compute(t);
            t+=dt;

            point = VP * point;
            point.x /= point.w;
            point.y /= point.w;

            if(point.w < 0) continue;
            glVertex2f(point.x, point.y);
        }

    }
    glEnd();
}


//-----------------------//
//  PUBLIC
//-----------------------//


void BezierCurve::addPoint(ic::Point* point){
    this->points.push_back(point);

    buildBezierCurves_C0();
}

void BezierCurve::removePoint(ic::Point* point){
    points.erase(remove(points.begin(), points.end(), point),
                 points.end());

    buildBezierCurves_C0();
}

void BezierCurve::swapPoints(int i, int j){
    if(i < 0 || j < 0 || i >= points.size() || j >= points.size()) return;

    Point* pointTMP = points[i];
    points[i] = points[j];
    points[j] = pointTMP;

    buildBezierCurves_C0();

}

void BezierCurve::moveUp(ic::Point* point){
    int i = getPointIndex(point);
    if(i <= 0) return;

    swapPoints(i, i-1);

    buildBezierCurves_C0();
}

void BezierCurve::moveDown(ic::Point* point){
    int i = getPointIndex(point);
    if(i < 0 || i == points.size() - 1) return;

    swapPoints(i, i+1);

    buildBezierCurves_C0();
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

void BezierCurve::render(const glm::mat4 &VP) {
    setSurfaceColor(this->color);
    drawCurves(VP);

    printStatusDEBUG();
}

void BezierCurve::render(const glm::mat4 &VP, const Color &color) {
    setSurfaceColor(color);
    drawCurves(VP);
}
