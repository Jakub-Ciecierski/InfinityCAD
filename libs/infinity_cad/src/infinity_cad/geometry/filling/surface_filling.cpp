//
// Created by jakub on 6/25/16.
//

#include <infinity_cad/geometry/filling/surface_filling.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/math/math.h>

using namespace std;
using namespace glm;

SurfaceFilling::SurfaceFilling(Surface* surface1,
                               Surface* surface2,
                               Surface* surface3,
                               Scene* scene) :
        scene(scene),
        fillingStatus(FillingStatus::COMPUTING){
    fillingData[0].surface = surface1;
    fillingData[1].surface = surface2;
    fillingData[2].surface = surface3;

    renderMode = RenderMode::DEBUG;

    debugColors.CommonPointColor = Color(1.0, 0.0, 0.0);
    debugColors.BorderControlPointsColor = Color(0.5, 0.0, 0.0);
    debugColors.BorderCurveMidPointColor = Color(1.0f, 1.0f, 0.0f);
    debugColors.BorderCurveMidTangetColor = Color(1.0f, 0.0f, 0.0f);

    debugColors.a0TangentColor = Color(1.0f, 1.0f, 0.0f);
    debugColors.a3TangentColor = Color(0.0f, 1.0f, 0.0f);
    debugColors.b3TangentColor = Color(0.0f, 0.0f, 1.0f);

    debugColors.P3Color = Color(0.0f, 0.0f, 0.8f);
    debugColors.P2Color = Color(0.8f, 0.0f, 0.0f);

    debugColors.CenterBezierColor = Color(1.0f, 1.0f, 1.0f);
}

SurfaceFilling::~SurfaceFilling() {

}

//-----------------------//
//  PRIVATE
//-----------------------//

void SurfaceFilling::findCommonPoints(){
    // Find common points
    ifc::Point* point12 = findCommonPoint(fillingData[0].surface,
                                          fillingData[1].surface);
    ifc::Point* point13 = findCommonPoint(fillingData[0].surface,
                                          fillingData[2].surface);
    ifc::Point* point23 = findCommonPoint(fillingData[1].surface,
                                          fillingData[2].surface);

    fillingData[0].commonPoint1 = point12;
    fillingData[0].commonPoint2 = point13;
    fillingData[0].left = &(fillingData[1]);
    fillingData[0].right = &(fillingData[2]);

    fillingData[1].commonPoint1 = point12;
    fillingData[1].commonPoint2 = point23;
    fillingData[1].left = &(fillingData[2]);
    fillingData[1].right = &(fillingData[0]);

    fillingData[2].commonPoint1 = point13;
    fillingData[2].commonPoint2 = point23;
    fillingData[2].left = &(fillingData[0]);
    fillingData[2].right = &(fillingData[1]);

    commonPoints.point12 = point12;
    commonPoints.point13 = point13;
    commonPoints.point23 = point23;

    updateStatusCommonPoints(commonPoints);
}

ifc::Point* SurfaceFilling::findCommonPoint(Surface* surface1,
                                            Surface* surface2){
    ifc::Point* commonPoint = NULL;

    const vector<ifc::Point*>& points1 = surface1->getAllPoints();
    const vector<ifc::Point*>& points2 = surface2->getAllPoints();

    for(unsigned int i = 0; i < points1.size(); i++){
        for(unsigned int j = 0; j < points2.size(); j++){
            if(points1[i] == points2[j]){
                commonPoint = points1[i];
            }
        }
    }
    return commonPoint;
}

void SurfaceFilling::updateStatusCommonPoints(CommonPoints &commonPoints){
    if(commonPoints.point12 == NULL || commonPoints.point13 == NULL ||
       commonPoints.point23 == NULL){
        fillingStatus = FillingStatus::NO_HOLE;
    }
}

void SurfaceFilling::findBorderCurves(){
    findBorderCurve(fillingData[0]);
    findBorderCurve(fillingData[1]);
    findBorderCurve(fillingData[2]);
}

void SurfaceFilling::findBorderCurve(FillingData &fillingData){
    const Matrix<ifc::Point*>& points = fillingData.surface->getMatrixPoints();
    const int FIRST = 0;
    const int LAST = 3;

    // Try by columns
    for(unsigned int i = 0; i < points.columnCount(); i++){
        vector<ifc::Point*> borderPoints = points.getColumn(i);
        if((borderPoints[FIRST] == fillingData.commonPoint1 &&
            borderPoints[LAST] == fillingData.commonPoint2) ||
           (borderPoints[LAST] == fillingData.commonPoint1 &&
            borderPoints[FIRST] == fillingData.commonPoint2)){

            fillingData.borderPoints = borderPoints;
            fillingData.varBorderCurveParam = BorderCurveParam::V;
            if(i == FIRST)
                fillingData.constBorderCurveParam = BorderCurveParam::U0;
            else
                fillingData.constBorderCurveParam = BorderCurveParam::U1;

            return;
        }
    }
    for(unsigned int i = 0; i < points.rowCount(); i++){
        vector<ifc::Point*> borderPoints = points[i];
        if((borderPoints[FIRST] == fillingData.commonPoint1 &&
            borderPoints[LAST] == fillingData.commonPoint2) ||
           (borderPoints[LAST] == fillingData.commonPoint1 &&
            borderPoints[FIRST] == fillingData.commonPoint2)){
            fillingData.borderPoints = borderPoints;
            fillingData.varBorderCurveParam = BorderCurveParam::U;

            if(i == FIRST)
                fillingData.constBorderCurveParam = BorderCurveParam::V0;
            else
                fillingData.constBorderCurveParam = BorderCurveParam::V1;

            return;
        }
    }
}

void SurfaceFilling::computeBorderCurveMidPoints(){
    computerBorderCurveMidPoint(fillingData[0]);
    computerBorderCurveMidPoint(fillingData[1]);
    computerBorderCurveMidPoint(fillingData[2]);
}

void SurfaceFilling::computerBorderCurveMidPoint(FillingData& fillingData){
    vec2 uv = getUV(fillingData);

    fillingData.P0_BorderCurveMidPoint
            = fillingData.surface->compute(uv.x, uv.y);
}

void SurfaceFilling::computeBorderCurveTangents(){
    computeBorderCurveTangent(fillingData[0]);
    computeBorderCurveTangent(fillingData[1]);
    computeBorderCurveTangent(fillingData[2]);
}

void SurfaceFilling::computeBorderCurveTangent(FillingData& fillingData){
    vec2 uv = getUV(fillingData);
    vec3 tangent;

    if(fillingData.varBorderCurveParam == BorderCurveParam::U){
        tangent = fillingData.surface->computeDv(uv.x, uv.y);
    }
    else if(fillingData.varBorderCurveParam == BorderCurveParam::V){
        tangent = fillingData.surface->computeDu(uv.x, uv.y);
    }

    if(fillingData.constBorderCurveParam == BorderCurveParam::V0 ||
            fillingData.constBorderCurveParam == BorderCurveParam::U0){
        tangent = -tangent;
    }

    tangent = normalize(tangent);
    tangent *= 0.33f;

    fillingData.borderCurveTanget = tangent;

    fillingData.P1_Tanget = fillingData.P0_BorderCurveMidPoint
                            + fillingData.borderCurveTanget;
}

void SurfaceFilling::computeCenterPoints(){
    vec3 Q1 = computeAuxiliaryPointQ(fillingData[0]);
    vec3 Q2 = computeAuxiliaryPointQ(fillingData[1]);
    vec3 Q3 = computeAuxiliaryPointQ(fillingData[2]);

    vec3 P3_CentralPoint = (Q1 + Q2 + Q3) / 3.0f;

    vec3 P2_1 = ((Q1 * 2.0f) + P3_CentralPoint) / 3.0f;
    vec3 P2_2 = ((Q2 * 2.0f) + P3_CentralPoint) / 3.0f;
    vec3 P2_3 = ((Q3 * 2.0f) + P3_CentralPoint) / 3.0f;

    fillingData[0].P2 = P2_1;
    fillingData[0].P3_Center = P3_CentralPoint;

    fillingData[1].P2 = P2_2;
    fillingData[1].P3_Center = P3_CentralPoint;

    fillingData[2].P2 = P2_3;
    fillingData[2].P3_Center = P3_CentralPoint;
}

vec3 SurfaceFilling::computeAuxiliaryPointQ(FillingData &fillingData){
    vec3& P0 = fillingData.P0_BorderCurveMidPoint;
    vec3& P1 = fillingData.P1_Tanget;

    vec3 Q = (P1*3.0f - P0) / 2.0f;

    return Q;
}

void SurfaceFilling::computeGFieldVectors(){
    computeGFieldVector(fillingData[0]);
    computeGFieldVector(fillingData[1]);
    computeGFieldVector(fillingData[2]);
}

void SurfaceFilling::computeGFieldVector(FillingData& fillingData){
    vec3 a0, b0, a3, b3;
    vec3 g0, g1, g2;

    vec2 uv = getUV(fillingData);

    if(fillingData.varBorderCurveParam == BorderCurveParam::U){
        a0 = fillingData.surface->computeDu(uv.x, uv.y);
    }
    else if(fillingData.varBorderCurveParam == BorderCurveParam::V){
        a0 = fillingData.surface->computeDv(uv.x, uv.y);
    }
    if(fillingData.constBorderCurveParam == BorderCurveParam::V0 ||
       fillingData.constBorderCurveParam == BorderCurveParam::U0){
        ;//a0 = -a0;
    }
    a0 = normalize(a0);
    a0 *= 0.33f;
    b0 = a0;

    a3 = fillingData.left->P3_Center - fillingData.left->P2;
    b3 = fillingData.right->P3_Center - fillingData.right->P2;

    a3 = -a3;
    b3 = -b3;

    a3 = normalize(a3);
    a3 *= 0.33f;
    b3 = normalize(b3);
    b3 *= 0.33f;

    fillingData.a0Tanget = a0;
    fillingData.b0Tanget = b0;
    fillingData.a3Tanget = a3;
    fillingData.b3Tanget = b3;

    fillingData.a0Point = fillingData.P0_BorderCurveMidPoint + a0;
    fillingData.b0Point = fillingData.P0_BorderCurveMidPoint + b0;
    fillingData.a3Point = fillingData.P3_Center + a3;
    fillingData.b3Point = fillingData.P3_Center + b3;
}

vec2 SurfaceFilling::getUV(FillingData& fillingData){
    const float HALF_P = 0.5f;
    const float START_P = 0.0f;
    const float END_P = 1.0f;
    const float NO_P = -1.0f;

    float u = NO_P;
    float v = NO_P;

    if(fillingData.varBorderCurveParam == BorderCurveParam::U) u = HALF_P;
    else v = HALF_P;

    if(fillingData.constBorderCurveParam == BorderCurveParam::U0) u = START_P;
    if(fillingData.constBorderCurveParam == BorderCurveParam::U1) u = END_P;
    if(fillingData.constBorderCurveParam == BorderCurveParam::V0) v = START_P;
    if(fillingData.constBorderCurveParam == BorderCurveParam::V1) v = END_P;

    if(u == NO_P || v == NO_P){
        throw new invalid_argument("Invalid u,v parameters");
    }

    return vec2(u,v);
}

void SurfaceFilling::renderDebug(){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    commonPoints.point12->setColor(debugColors.CommonPointColor);
    commonPoints.point13->setColor(debugColors.CommonPointColor);
    commonPoints.point23->setColor(debugColors.CommonPointColor);

    for(unsigned int j = 0; j < 3; j++) {
        std::cout << "Surface:     "
        << fillingData[j].surface->getName() << std::endl;

        std::cout << "Var Param:   "
        << borderCurveParamToString(fillingData[j].varBorderCurveParam)
        << std::endl;
        std::cout << "Const Param: "
        << borderCurveParamToString(fillingData[j].constBorderCurveParam)
        << std::endl;
        std::cout << " ------------ " << std::endl << std::endl;
        for (unsigned int i = 0; i < fillingData[j].borderPoints.size(); i++) {
            fillingData[j].borderPoints[i]->
                    setColor(debugColors.BorderControlPointsColor);
        }

        // P0 Middle Point
        ifc::Point* P0 = objectFactory.createPoint(to_string(j));
        P0->moveTo(fillingData[j].P0_BorderCurveMidPoint);
        P0->setColor(debugColors.BorderCurveMidPointColor);

        // P1: Tangent
        vec3 midPos = P0->getPosition();
        vec3 v2 = fillingData[j].P1_Tanget;
        vec4 v1 = vec4(midPos.x, midPos.y, midPos.z, 1.0f);
        vec4 v2_4 = vec4(v2.x, v2.y, v2.z, 1.0f);
        Line* tangetLine = objectFactory.createLine("line", v1, v2_4);
        tangetLine->setColor(debugColors.BorderCurveMidTangetColor);

        ifc::Point* P1 = objectFactory.createPoint(to_string(j));
        P1->moveTo(fillingData[j].P1_Tanget);
        P1->setColor(debugColors.P2Color);

        // P2
        ifc::Point* P2 = objectFactory.createPoint(to_string(j));
        P2->moveTo(fillingData[j].P2);
        P2->setColor(debugColors.P2Color);

        // P3
        ifc::Point* P3 = objectFactory.createPoint(to_string(j));
        P3->moveTo(fillingData[j].P3_Center);
        P3->setColor(debugColors.P3Color);

        // a0, a3, b3
        vec3 a0 = fillingData[j].a0Point;
        vec3 a3 = fillingData[j].a3Point;
        vec3 b3 = fillingData[j].b3Point;

        vec4 vv2 = vec4(a0.x, a0.y, a0.z, 1.0f);
        Line* a0Line = objectFactory.createLine("line", v1, vv2);
        a0Line->setColor(debugColors.a0TangentColor);

        vec3 center = fillingData[j].P3_Center;
        vec4 center4 = vec4(center.x, center.y, center.z, 1.0f);
        vec4 a3_4 = vec4(a3.x, a3.y, a3.z, 1.0f);
        vec4 b3_4 = vec4(b3.x, b3.y, b3.z, 1.0f);

        Line* a3Line = objectFactory.createLine("line", center4, a3_4);
        a3Line->setColor(debugColors.a3TangentColor);
        Line* b3Line = objectFactory.createLine("line", center4, b3_4);
        b3Line->setColor(debugColors.b3TangentColor);

        // Bezier
        vector<ifc::Point*> bezierPoints{P0, P1, P2, P3};
        BezierSplineC0* bezierSpline = objectFactory.createBezier("bezier",
                                                                  bezierPoints);
        bezierSpline->setColor(debugColors.CenterBezierColor);

        scene->addRenderObject(P0);
        scene->addRenderObject(P1);
        scene->addRenderObject(P2);
        scene->addRenderObject(P3);
        scene->addRenderObject(tangetLine);

        if(j == 0){
            scene->addRenderObject(a0Line);
            scene->addRenderObject(a3Line);
            scene->addRenderObject(b3Line);
        }

        scene->addRenderObject(bezierSpline);
    }
}

std::string SurfaceFilling::borderCurveParamToString(BorderCurveParam param){
    if(param == BorderCurveParam::U) return "U";
    if(param == BorderCurveParam::V) return "V";
    if(param == BorderCurveParam::U0) return "U0";
    if(param == BorderCurveParam::U1) return "U1";
    if(param == BorderCurveParam::V0) return "V0";
    if(param == BorderCurveParam::V1) return "V1";
    return "Unknown";
}

//-----------------------//
//  PUBLIC
//-----------------------//

FillingStatus SurfaceFilling::getStatus(){
    return this->fillingStatus;
}

void SurfaceFilling::start(){
    findCommonPoints();
    if(fillingStatus != FillingStatus::COMPUTING) return;

    findBorderCurves();

    computeBorderCurveMidPoints();
    computeBorderCurveTangents();
    computeCenterPoints();
    computeGFieldVectors();

    if(renderMode == RenderMode::DEBUG) renderDebug();
}