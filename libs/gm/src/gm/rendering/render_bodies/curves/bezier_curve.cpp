//
// Created by jakub on 4/5/16.
//

#include <gm/rendering/render_bodies/curves/bezier_curve.h>
#include <algorithm>
#include <iostream>
#include <gm/rendering/clipping/sutherland.h>
#include <gm/scene/object_factory.h>

using namespace ic;
using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BezierCurve::BezierCurve(SceneID id, std::string name) :
        RenderBody(id, name){
    //this->color = Color(0.8, 0, 0, 1);
    this->polygonColor = Color(1,1,1,1);

    grabable = false;
    setDrawBezierPolygon(true);
}

BezierCurve::BezierCurve(SceneID id, std::string name,
        std::vector<ic::Point*>& points) :
        RenderBody(id, name){
    for(unsigned int i = 0;i < points.size(); i++){
        this->points.push_back(points[i]);
    }
    grabable = false;
    setDrawBezierPolygon(false);

    buildBezierCurves_C0();
}

BezierCurve::~BezierCurve(){

}


//-----------------------//
//  PRIVATE
//-----------------------//


void BezierCurve::buildBezierCurves_C0(){
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
        if(addNewCurve){
            currentSegmentIndex++;
            bezierCurves.push_back(BezierCubicCurve());
        }

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
}

void BezierCurve::draw(const glm::mat4 &VP){
    drawCurves(VP);
    if(doDrawBezierPolygon)
        drawBezierPolygon(VP);
}

void BezierCurve::drawBezierPolygon(const glm::mat4 &VP, int SEGMENTS){
    unsigned int bezierCurvesCount = bezierCurves.size();

    Color color(1,1,1,1);
    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);

    glBegin(GL_LINES);
    for(unsigned int i = 0;i < bezierCurvesCount; i++){
        BezierCubicCurve& bezierCurveCubic = bezierCurves[i];
        int degree = bezierCurveCubic.degree();
        if(degree <= 0 ) return;

        ObjectFactory& objectFactory = ObjectFactory::getInstance();

        for(int i = 0; i < degree; i++){
            const ic::Point* point1 = bezierCurveCubic.getPoint(i);
            const ic::Point* point2 = bezierCurveCubic.getPoint(i+1);

            const vec3& pos1 = point1->getPosition();
            const vec3& pos2 = point2->getPosition();

            vec4 v1 = vec4(pos1.x, pos1.y, pos1.z, 1);
            vec4 v2 = vec4(pos2.x, pos2.y, pos2.z, 1);

            float segDelta = 1 / (float)SEGMENTS;
            float currSeg = 0;

            while(currSeg <= 1){
                vec4 v = v1 + currSeg*(v2-v1);
                currSeg += segDelta;

                Line* line = objectFactory.createLine("line", v1, v);
                line->update();
                line->render(VP, polygonColor);
                delete line;
            }

            // clipping hack TODO
            currSeg = 0;
            while(currSeg <= 1){
                vec4 v = v2 + currSeg*(v1-v2);
                currSeg += segDelta;

                Line* line = objectFactory.createLine("line", v, v2);
                line->update();
                line->render(VP, polygonColor);
                delete line;
            }
        }
    }
    glEnd();
}

void BezierCurve::drawCurves(const glm::mat4 &VP) {
    unsigned int bezierCurvesCount = bezierCurves.size();

    int PIXELS_COUNT = 10000;
    float dt = 1/(float)PIXELS_COUNT;

    setSurfaceColor(color);
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

            if(point.w < 0) continue; // clip
            glVertex2f(point.x, point.y);
        }
    }
    glEnd();
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

void BezierCurve::setDrawBezierPolygon(bool value){
    this->doDrawBezierPolygon = value;
}

bool BezierCurve::isDrawBezierPolygon(){
    return this->doDrawBezierPolygon;
}

float BezierCurve::intersect(const RayCast &ray){
    return RAYCAST_NO_SOLUTION;
}

glm::vec3 BezierCurve::getClosestPoint(const glm::vec3 point){
    return vec3(0,0,0);
}

void BezierCurve::render(const glm::mat4 &VP) {
    draw(VP);
}

void BezierCurve::render(const glm::mat4 &VP, const Color &color) {
    draw(VP);
}
