//
// Created by jakub on 4/13/16.
//


#include <algorithm>
#include <infinity_cad/rendering/render_objects/curves/bezier_spline_c2.h>
#include <infinity_cad/rendering/scene/object_factory.h>


using namespace ifc;
using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BezierSplineC2::BezierSplineC2(SceneID id, std::string name) :
        Spline(id, name){
    //this->color = Color(0.8, 0, 0, 1);
    this->polygonColor = Color(1,1,1,1);

    grabable = false;
    setDrawBezierPolygon(false);
    dt = -1;

    objectType = OBJ_TYPE_CURVE_BSPLINE;
}

BezierSplineC2::BezierSplineC2(SceneID id, std::string name,
                               std::vector<ifc::Point*>& points) :
        Spline(id, name){
    for(unsigned int i = 0;i < points.size(); i++){
        this->points.push_back(points[i]);
        this->addChild(points[i]); // TODO children mechanism
    }
    grabable = false;
    setDrawBezierPolygon(false);

    buildCurve();

    dt = -1;

    objectType = OBJ_TYPE_CURVE_BSPLINE;
}

BezierSplineC2::~BezierSplineC2(){

}

//-----------------------//
//  PRIVATE
//-----------------------//

void BezierSplineC2::C2Continuity(){

    // http://www.cse.iitd.ernet.in/~pkalra/csl859/slides/bezier2.PDF
    // C1
    //int i = 0;
    if(bezierCurves.size() == 1) return;

    // C1

    for(unsigned int i = 0;i < bezierCurves.size() - 1; i++){
        BezierCubicCurve& curve1 = bezierCurves[i];
        BezierCubicCurve& curve2 = bezierCurves[i+1];

        ifc::Point* p3 = curve1.getPoint(3);
        ifc::Point* p2 = curve1.getPoint(2);

        ifc::Point* q1 = curve2.getPoint(1);

        const vec3& posP3 = p3->getPosition();
        const vec3& posP2 = p2->getPosition();
        const vec3& posQ1 = q1->getPosition();

        //if(!p2->isSelected() && !q1->isSelected()) return;

        if(p3->isSelected()){
            vec3 newPosP2 = posP3 * 2.0f - posQ1;
            p2->moveTo(newPosP2);

            vec3 newPosQ1 = posP3 * 2.0f - posP2;
            q1->moveTo(newPosQ1);
        }

        if(!p2->isSelected()) {
            vec3 newPosP2 = posP3 * 2.0f - posQ1;
            p2->moveTo(newPosP2);
        }
        else if(!q1->isSelected()) {
            vec3 newPosQ1 = posP3 * 2.0f - posP2;
            q1->moveTo(newPosQ1);
        }

    }
    // C2

    for(unsigned int i = 0;i < bezierCurves.size() - 1; i++){
        BezierCubicCurve& curve1 = bezierCurves[i];
        BezierCubicCurve& curve2 = bezierCurves[i+1];

        ifc::Point* p3 = curve1.getPoint(3);
        ifc::Point* p2 = curve1.getPoint(2);
        ifc::Point* p1 = curve1.getPoint(1);

        ifc::Point* q1 = curve2.getPoint(1);
        ifc::Point* q2 = curve2.getPoint(2);

        const vec3& posP3 = p3->getPosition();
        const vec3& posP2 = p2->getPosition();
        const vec3& posP1 = p1->getPosition();

        const vec3& posQ1 = q1->getPosition();
        const vec3& posQ2 = q2->getPosition();

        //if(!q2->isSelected() && !p1->isSelected()) continue;

        if(!q2->isSelected()) {
            vec3 newPos1 = 4.0f * (posP3 - posP2) + posP1;
            q2->moveTo(newPos1);

        }
        else if(!p1->isSelected()){
            vec3 newPos1 = 4.0f * (posP3 - posQ1) + posQ2;
            p1->moveTo(newPos1);
        }

    }
}

void BezierSplineC2::initVertices(){

}

void BezierSplineC2::initEdges(){

}

void BezierSplineC2::buildCurve(){
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

void BezierSplineC2::draw(const glm::mat4 &VP, const Color& color){
    drawCurves(VP, color);
    if(doDrawBezierPolygon)
        drawBezierPolygon(VP);
}

void BezierSplineC2::drawBezierPolygon(const glm::mat4 &VP, int SEGMENTS){
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

void BezierSplineC2::drawCurves(const glm::mat4 &VP, const Color& color) {
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
        if(this->dt < dt) this->dt = dt;

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
//  PUBLIC
//-----------------------//

float BezierSplineC2::getDT(){
    return dt;
}

std::vector<BezierCubicCurve>&BezierSplineC2::getCurves(){
    return this->bezierCurves;
}

void BezierSplineC2::render(const glm::mat4 &VP) {
    draw(VP, this->color);
    for(unsigned int i = 0; i < children.size(); i++){
        children[i]->render(VP);
    }
}

void BezierSplineC2::render(const glm::mat4 &VP, const Color &color) {
    for(unsigned int i = 0; i < children.size(); i++){
        Color c(1,0,0); // red
        children[i]->render(VP, c);
    }
    draw(VP, color);

}

void BezierSplineC2::update() {
    for(unsigned int i = 0; i < children.size(); i++){
        children[i]->update();
    }
    C2Continuity();
}
