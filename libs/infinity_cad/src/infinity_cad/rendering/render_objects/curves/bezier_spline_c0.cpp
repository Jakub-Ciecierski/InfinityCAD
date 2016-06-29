//
// Created by jakub on 4/5/16.
//

#include <algorithm>
#include <iostream>
#include <infinity_cad/rendering/render_objects/curves/bezier_spline_c0.h>
#include <infinity_cad/rendering/scene/object_factory.h>


using namespace ifc;
using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BezierSplineC0::BezierSplineC0(SceneID id, std::string name) :
        Spline(id, name){
    //this->color = Color(0.8, 0, 0, 1);
    this->polygonColor = Color(1,1,1,1);

    grabable = false;
    setDrawBezierPolygon(false );

    objectType = OBJ_TYPE_CURVE_BEZIER;
}

BezierSplineC0::BezierSplineC0(SceneID id, std::string name,
                               std::vector<ifc::Point*>& points) :
        Spline(id, name){
    for(unsigned int i = 0;i < points.size(); i++){
        this->points.push_back(points[i]);
    }
    grabable = false;
    setDrawBezierPolygon(false);

    buildCurve();

    objectType = OBJ_TYPE_CURVE_BEZIER;
}

BezierSplineC0::~BezierSplineC0(){

}


//-----------------------//
//  PRIVATE
//-----------------------//


void BezierSplineC0::buildCurve(){
    int pointsCount = points.size();
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

void BezierSplineC0::draw(const glm::mat4 &VP, const Color& color){
    drawCurves(VP, color);
    if(doDrawBezierPolygon)
        drawBezierPolygon(VP);
}

void BezierSplineC0::drawBezierPolygon(const glm::mat4 &VP, int SEGMENTS){
    unsigned int bezierCurvesCount = bezierCurves.size();

    Color color(1,1,1,1);
    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);

    glBegin(GL_LINES);
    for(unsigned int i = 0;i < bezierCurvesCount; i++){
        BezierCubicCurve& bezierCurveCubic = bezierCurves[i];
        int degree = bezierCurveCubic.degree();
        if(degree <= 0 ) break;

        ObjectFactory& objectFactory = ObjectFactory::getInstance();

        for(int i = 0; i < degree; i++){
            const ifc::Point* point1 = bezierCurveCubic.getPoint(i);
            const ifc::Point* point2 = bezierCurveCubic.getPoint(i+1);

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

void BezierSplineC0::drawCurves(const glm::mat4 &VP, const Color& color) {
    unsigned int bezierCurvesCount = bezierCurves.size();

    int sumScreen = 4 * (screenWidth + screenHeight);

    setSurfaceColor(color);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for(unsigned int i = 0;i < bezierCurvesCount; i++){
        BezierCubicCurve& bezierCurveCubic = bezierCurves[i];
        float maxDistance = bezierCurveCubic.getMaximumDistanceBetweenPoints();
        if(maxDistance < 0) maxDistance = 1;

        float dt = 1/((float)sumScreen * maxDistance);

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

void BezierSplineC0::initVertices(){

}

void BezierSplineC0::initEdges(){

}


//-----------------------//
//  PUBLIC
//-----------------------//

glm::vec3 BezierSplineC0::compute(float t){
    // ASSUMES IT HAS ONLY ONE CURVE
    BezierCubicCurve& bezierCurveCubic = bezierCurves[0];

    vec4 point4 = bezierCurveCubic.compute(t);

    vec3 point3 = vec3(point4.x, point4.y, point4.z);

    return point3;
}

glm::vec4 BezierSplineC0::computeVec4(float t){
    // ASSUMES IT HAS ONLY ONE CURVE
    BezierCubicCurve& bezierCurveCubic = bezierCurves[0];

    vec4 point4 = bezierCurveCubic.compute(t);

    return point4;
}