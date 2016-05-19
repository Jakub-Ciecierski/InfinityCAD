//
// Created by jakub on 4/10/16.
//


#include <infinity_cad/rendering/render_objects/curves/bspline.h>
#include <infinity_cad/geometry/polynomials/bspline_basis.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <iostream>
#include <infinity_cad/settings/settings.h>
#include <ifc_gpu/splines/bspline_gpu.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BSpline::BSpline(SceneID id, std::string name) :
    Spline(id, name){
    bezierSpline = NULL;

    setDrawBezierBasis(false);
    setDrawBezierPolygon(false);

    curvePoints = (vec4 *) malloc(MAX_PIXEL_COUNT * sizeof(vec4));

    objectType = OBJ_TYPE_CURVE_BSPLINE;
}

BSpline::~BSpline(){
    if(bezierSpline != NULL) delete bezierSpline;
    for(unsigned int i = 0 ;i < points.size();i++){
        points[i]->setShow(true);
    }
    delete curvePoints;
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
    if(getControlPointCount() < 4) return;

    if(drawBezierBasis){
        drawBezierSpline(VP, color);
        /*
        for(unsigned int i = 0 ;i < points.size();i++){
            points[i]->setShow(false);
        }*/
    }else {
        /*
        for(unsigned int i = 0 ;i < points.size();i++){
            points[i]->setShow(true);
        }*/
        drawSpline(VP, color);
    }
    if(doDrawBezierPolygon)
        drawPolygon(VP);
}

void BSpline::drawBezierSpline(const glm::mat4 &VP, const Color& color){
    bezierSpline->render(VP, color);

    int segmentCount = getControlPointCount() - getDegree();

    Color c(0,1,0);
    setSurfaceColor(c);
    glPointSize(5.0f);
    glBegin(GL_POINTS);

    vector<BezierCubicCurve>& curves = bezierSpline->getCurves();
    for(int i = 0; i < segmentCount; i++){
        BezierCubicCurve& curve = curves[i];

        ifc::Point* bezierPoint0 = curve.getPoint(0);
        ifc::Point* bezierPoint1 = curve.getPoint(1);
        ifc::Point* bezierPoint2 = curve.getPoint(2);
        ifc::Point* bezierPoint3 = curve.getPoint(3);

        const vec3& bezierPos0 = bezierPoint0->getPosition();
        const vec3& bezierPos1 = bezierPoint1->getPosition();
        const vec3& bezierPos2 = bezierPoint2->getPosition();
        const vec3& bezierPos3 = bezierPoint3->getPosition();

        vec4 boorPos0(6.0f*bezierPos0 - 7.0f*bezierPos1 + 2.0f*bezierPos2, 1);
        vec4 boorPos1(2.0f*bezierPos1 - bezierPos2, 1);
        vec4 boorPos2(-bezierPos1 + 2.0f*bezierPos2, 1);
        vec4 boorPos3(2.0f*bezierPos1 - 7.0f*bezierPos2 + 6.0f*bezierPos3, 1);

        points[i+0]->moveTo(boorPos0.x, boorPos0.y, boorPos0.z);
        points[i+1]->moveTo(boorPos1.x, boorPos1.y, boorPos1.z);
        points[i+2]->moveTo(boorPos2.x, boorPos2.y, boorPos2.z);
        points[i+3]->moveTo(boorPos3.x, boorPos3.y, boorPos3.z);

        boorPos0 = VP * boorPos0;
        boorPos0.x /= boorPos0.w;
        boorPos0.y /= boorPos0.w;
        if(boorPos0.w > 0)// clip
            glVertex2f(boorPos0.x, boorPos0.y);

        boorPos1 = VP * boorPos1;
        boorPos1.x /= boorPos1.w;
        boorPos1.y /= boorPos1.w;
        if(boorPos1.w > 0)// clip
            glVertex2f(boorPos1.x, boorPos1.y);

        boorPos2 = VP * boorPos2;
        boorPos2.x /= boorPos2.w;
        boorPos2.y /= boorPos2.w;
        if(boorPos2.w > 0)// clip
            glVertex2f(boorPos2.x, boorPos2.y);

        boorPos3 = VP * boorPos3;
        boorPos3.x /= boorPos3.w;
        boorPos3.y /= boorPos3.w;
        if(boorPos3.w > 0)// clip
            glVertex2f(boorPos3.x, boorPos3.y);
    }
    glEnd();
}

void BSpline::drawSpline(const glm::mat4 &VP, const Color& color){
    int degree = computeDegree();
    if(degree == -1) return;

    float dt = -1;
    if(bezierSpline != NULL)
        dt = bezierSpline->getDT();
    if(dt == -1)
        dt = 0.0001;

    float t_max = getKnotMax();
    float t = getKnotMin(degree);

    int pixelCount = (t_max - t) / dt;

    if(ifc::RUN_CUDA){
        drawSplineGPU(VP, color, pixelCount, t, dt, degree);
    }else{
        drawSplineCPU(VP, color, degree, t, t_max, dt);
    }

    // ---------

    Color c(1,0,0);
    setSurfaceColor(c);
    glPointSize(5.0f);
    glBegin(GL_POINTS);

    vector<vec4> bezierPoints = this->calculateBezierPoints();

    for(unsigned int i = 0; i < bezierPoints.size();i++){
        bezierPoints[i] = VP * bezierPoints[i];
        bezierPoints[i].x /= bezierPoints[i].w;
        bezierPoints[i].y /= bezierPoints[i].w;

        if(bezierPoints[i].w > 0)// clip
            glVertex2f(bezierPoints[i].x, bezierPoints[i].y);
    }
    glEnd();

}

void BSpline::drawSplineCPU(const glm::mat4 &VP, const Color& color,
                            int degree, float t, float t_max, float dt){
    setSurfaceColor(color);
    glPointSize(1.0f);
    glBegin(GL_POINTS);

    while(t < t_max){
        vec4 point = computeBSpline(points, knotVector, t, degree);
        t += dt;

        point = VP * point;
        point.x /= point.w;
        point.y /= point.w;

        if(point.w < 0) continue; // clip
        glVertex2f(point.x, point.y);
    }

    glEnd();
}
void BSpline::drawSplineGPU(const glm::mat4 &VP, const Color& color,
                            int pixelCount, float t, float dt, int degree){
    int pointSize = points.size();
    std::vector<vec3> posPoints(pointSize);
    for(int i = 0;i < pointSize; i++){
        posPoints[i] = points[i]->getPosition();
    }

    if(pixelCount > MAX_PIXEL_COUNT){
        delete curvePoints;
        MAX_PIXEL_COUNT = pixelCount;
        curvePoints = (vec4*) malloc(MAX_PIXEL_COUNT * sizeof(vec4));
    }

    ifc_gpu::computeBSpline(posPoints.data(), pointSize,
                            knotVector.data(), knotVector.size(),
                            curvePoints, pixelCount, t, dt, degree, &VP);

    setSurfaceColor(color);
    glPointSize(1.0f);
    glBegin(GL_POINTS);

    for(int i = 0; i < pixelCount; i++){
        if(curvePoints[i].w < 0) continue; // clip
        glVertex2f(curvePoints[i].x, curvePoints[i].y);
    }

    glEnd();
}

void BSpline::drawPolygon(const glm::mat4 &VP, int SEGMENTS){
    Color color(0.6,0.6,0.6,1);
    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);

    glBegin(GL_LINES);
    for(unsigned int i = 0; i < points.size() - 1; i++){
        ObjectFactory& objectFactory = ObjectFactory::getInstance();

        const ifc::Point* point1 = points[i];
        const ifc::Point* point2 = points[i+1];

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
            line->render(VP, color);
            delete line;
        }

        // clipping hack TODO
        currSeg = 0;
        while(currSeg <= 1){
            vec4 v = v2 + currSeg*(v1-v2);
            currSeg += segDelta;

            Line* line = objectFactory.createLine("line", v, v2);
            line->update();
            line->render(VP, color);
            delete line;
        }

    }
    glEnd();
}

vector<vec4> BSpline::calculateBezierPoints(){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    int segmentCount = getControlPointCount() - getDegree();
    vector<vec4> bezierPoints;
    for(int i = 0; i < segmentCount; i++){
        const vec3& pos0 = points[i+0]->getPosition();
        const vec3& pos1 = points[i+1]->getPosition();
        const vec3& pos2 = points[i+2]->getPosition();
        const vec3& pos3 = points[i+3]->getPosition();

        bezierPoints.push_back(vec4((pos0 + pos1*4.0f + pos2)/6.0f, 1));
        bezierPoints.push_back(vec4((pos1*4.0f + pos2*2.0f)/6.0f, 1));
        bezierPoints.push_back(vec4((pos1*2.0f + pos2*4.0f)/6.0f, 1));
        bezierPoints.push_back(vec4((pos1 + pos2*4.0f + pos3)/6.0f, 1));
    }
    return bezierPoints;
}

void BSpline::buildBezierSpline(){
    if(getControlPointCount() < 4) return;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    int segmentCount = getControlPointCount() - getDegree();
    //vector<vec4> bezierPoints;
    vector<ifc::Point*> bezierPoints;
    for(int i = 0; i < segmentCount; i++){
        const vec3& pos0 = points[i+0]->getPosition();
        const vec3& pos1 = points[i+1]->getPosition();
        const vec3& pos2 = points[i+2]->getPosition();
        const vec3& pos3 = points[i+3]->getPosition();

        vec3 deboorPos0((pos0 + pos1*4.0f + pos2)/6.0f);
        vec3 deboorPos1((pos1*4.0f + pos2*2.0f)/6.0f);
        vec3 deboorPos2((pos1*2.0f + pos2*4.0f)/6.0f);
        vec3 deboorPos3((pos1 + pos2*4.0f + pos3)/6.0f);

        if(i == 0) {
            ifc::Point *point0 = objectFactory.createPoint("tmp");
            point0->moveTo(deboorPos0);
            bezierPoints.push_back(point0);
        }
        ifc::Point* point1 = objectFactory.createPoint("tmp");
        point1->moveTo(deboorPos1);
        bezierPoints.push_back(point1);
        ifc::Point* point2 = objectFactory.createPoint("tmp");
        point2->moveTo(deboorPos2);
        bezierPoints.push_back(point2);
        ifc::Point* point3 = objectFactory.createPoint("tmp");
        point3->moveTo(deboorPos3);
        bezierPoints.push_back(point3);
    }

    if(bezierSpline != NULL) delete bezierSpline;
    bezierSpline = objectFactory.createBezierSpline("tmp", bezierPoints);
}

void BSpline::buildCurve() {
    knotVector.clear();

    int n = this->getControlPointCount();
    int d = this->getDegree();
    int knotCount = n + d + 1;

    for(int i = 0; i < knotCount; i++){
        knotVector.push_back(i);
    }

    buildBezierSpline();
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

bool BSpline::isDrawBezierBasis(){
    return this->drawBezierBasis;
}

void BSpline::setDrawBezierBasis(bool value){
    if(value)  // TODO big hack
        buildBezierSpline();

    this->drawBezierBasis = value;

    if(this->bezierSpline != NULL)
        this->bezierSpline->setDrawBezierPolygon(this->isDrawBezierPolygon());
}

int BSpline::getDegree(){
    return computeDegree();
}

void BSpline::setDrawBezierPolygon(bool value) {
    Spline::setDrawBezierPolygon(value);

    if(this->bezierSpline != NULL)
        this->bezierSpline->setDrawBezierPolygon(value);
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
}

float BSpline::getKnotMax(){
    int size = knotVector.size();
    int index = getControlPointCount();
    if(index >= size){
        return -1;
    }
    return knotVector[index];
}

const std::vector<float>& BSpline::getKnotVector(){
    return this->knotVector;
}

const std::vector<RenderObject *> &BSpline::getChildren() {
    if(bezierSpline != NULL)
        return bezierSpline->getChildren();
    else
        return RenderObject::getChildren();
}

void BSpline::update() {
    if(bezierSpline != NULL) bezierSpline->update();
}


