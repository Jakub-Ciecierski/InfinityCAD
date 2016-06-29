//
// Created by jakub on 6/25/16.
//

#include <infinity_cad/geometry/filling/surface_filling.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/math/math.h>
#include <infinity_cad/geometry/polynomials/bernstein_basis.h>

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

    debugColors.GGieldColor = Color(1.0f, 0.5f, 0.0f);

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
    // TODO, dynamic left/right
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

    computeGFieldVectorTop(fillingData[0]);
    computeGFieldVectorTop(fillingData[1]);
    computeGFieldVectorTop(fillingData[2]);

    computeD(fillingData[0]);
    computeD(fillingData[1]);
    computeD(fillingData[2]);
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
    if(fillingData.constBorderCurveParam == BorderCurveParam::V1 ||
       fillingData.constBorderCurveParam == BorderCurveParam::U1){
        a0 = -a0;
    }
    a0 = normalize(a0);
    a0 *= 0.33f;
    b0 = a0;

    a3 = fillingData.left->P3_Center - fillingData.left->P2;
    b3 = fillingData.right->P3_Center - fillingData.right->P2;

    a3 = -a3;
    //b3 = -b3;

    a3 = normalize(a3);
    a3 *= 0.33f;
    b3 = normalize(b3);
    b3 *= 0.33f;

    g0 = (a0 + b0) / 2.0f;
    g2 = (a3 + b3) / 2.0f;
    g1 = (g0 + g2) / 2.0f;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    ifc::Point* P0
            = objectFactory.createPoint("point",
                                        fillingData.P0_BorderCurveMidPoint);
    ifc::Point* P1 = objectFactory.createPoint("point", fillingData.P1_Tanget);
    ifc::Point* P2 = objectFactory.createPoint("point", fillingData.P2);
    ifc::Point* P3 = objectFactory.createPoint("point", fillingData.P3_Center);

    vector<ifc::Point*> bezierPoints{P0, P1, P2, P3};
    BezierSplineC0* bezierSpline = objectFactory.createBezier("bezier",
                                                              bezierPoints);
    vec3 C0 = bezierSpline->compute(0.0f);
    vec3 C05 = bezierSpline->compute(0.5f);
    vec3 C1 = bezierSpline->compute(1.0f);

    fillingData.g0PointLeft = C0 + g0;
    fillingData.g1PointLeft = C05 + g1;
    fillingData.g2PointLeft = C1 + g2;

    fillingData.g0TangentLeft = g0;
    fillingData.g1TangentLeft = g1;
    fillingData.g2TangentLeft = g2;

    fillingData.a0TangetLeft = a0;
    fillingData.b0TangetLeft = b0;
    fillingData.a3TangetLeft = a3;
    fillingData.b3TangetLeft = b3;

    fillingData.a0PointLeft = fillingData.P0_BorderCurveMidPoint + a0;
    fillingData.b0PointLeft = fillingData.P0_BorderCurveMidPoint + b0;
    fillingData.a3PointLeft = fillingData.P3_Center + a3;
    fillingData.b3PointLeft = fillingData.P3_Center - b3;

    delete P0;
    delete P1;
    delete P2;
    delete P3;
    delete bezierSpline;
}

void SurfaceFilling::computeGFieldVectorTop(FillingData& fillingDataBase){
    FillingData& fillingData = *(fillingDataBase.left);
    vec3 a0, b0, a3, b3;
    vec3 g0, g1, g2;

    vec2 uv = getUV(fillingData);

    if(fillingData.varBorderCurveParam == BorderCurveParam::U){
        a0 = fillingData.surface->computeDu(uv.x, uv.y);
    }
    else if(fillingData.varBorderCurveParam == BorderCurveParam::V){
        a0 = fillingData.surface->computeDv(uv.x, uv.y);
    }
    if(fillingData.constBorderCurveParam == BorderCurveParam::V1 ||
       fillingData.constBorderCurveParam == BorderCurveParam::U1){
        a0 = -a0;
    }
    a0 = normalize(a0);
    a0 *= 0.33f;
    b0 = a0;

    a3 = fillingData.left->P3_Center - fillingData.left->P2;
    b3 = fillingData.right->P3_Center - fillingData.right->P2;

    a3 = -a3;
    //b3 = -b3;

    a3 = normalize(a3);
    a3 *= 0.33f;
    b3 = normalize(b3);
    b3 *= 0.33f;

    g0 = (a0 + b0) / 2.0f;
    g2 = (a3 + b3) / 2.0f;
    g1 = (g0 + g2) / 2.0f;

    //g0 = -g0;
    //g1 = -g1;
    //g2 = -g2;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    ifc::Point* P0
            = objectFactory.createPoint("point",
                                        fillingData.P0_BorderCurveMidPoint);
    ifc::Point* P1 = objectFactory.createPoint("point", fillingData.P1_Tanget);
    ifc::Point* P2 = objectFactory.createPoint("point", fillingData.P2);
    ifc::Point* P3 = objectFactory.createPoint("point", fillingData.P3_Center);

    vector<ifc::Point*> bezierPoints{P0, P1, P2, P3};
    BezierSplineC0* bezierSpline = objectFactory.createBezier("bezier",
                                                              bezierPoints);
    vec3 C0 = bezierSpline->compute(0.0f);
    vec3 C05 = bezierSpline->compute(0.5f);
    vec3 C1 = bezierSpline->compute(1.0f);

    fillingData.g0PointTop = C0 + g0;
    fillingData.g1PointTop = C05 + g1;
    fillingData.g2PointTop = C1 + g2;

    fillingData.a0PointTop = fillingData.P0_BorderCurveMidPoint + a0;
    fillingData.b0PointTop = fillingData.P0_BorderCurveMidPoint + b0;
    fillingData.a3PointTop = fillingData.P3_Center + a3;
    fillingData.b3PointTop = fillingData.P3_Center - b3;

    delete P0;
    delete P1;
    delete P2;
    delete P3;
    delete bezierSpline;
}


void SurfaceFilling::computeD(FillingData &fillingData){
    cout << "--- FillingData ---" << endl << endl;
    vec3 b0 = fillingData.b0PointLeft;
    vec3 b3 = fillingData.b3PointLeft;

    vec3 g0 = fillingData.g0PointLeft;
    vec3 g2 = fillingData.g2PointLeft;

    vec3 c0 = fillingData.P1_Tanget - fillingData.P0_BorderCurveMidPoint;
    vec3 c1 = fillingData.P2 - fillingData.P1_Tanget;
    vec3 c2 = fillingData.P3_Center - fillingData.P2;

    auto solve_c = [c0, c1, c2](float t){
        vec3 v = quadraticBernstein3(c0, c1, c2, t);
        return v;
    };

    auto solve_g = [fillingData](float t){
        vec3 v = quadraticBernstein3(fillingData.g0PointLeft,
                                     fillingData.g1PointLeft,
                                     fillingData.g2PointLeft, t);
        return v;
    };

    auto solve_KH = [](vec3& b, vec3& g, vec3& c){
        auto solve_KH_RHS = [](float k, float h, vec3& gg, vec3& cc){
            float x = (k * gg.x) + (h * cc.x);
            float y = (k * gg.y) + (h * cc.y);
            float z = (k * gg.z) + (h * cc.z);

            return vec3(x,y,z);
        };

        auto solve_k = [](float h, float b, float c, float g){
            float k = (b - (h*c)) / g;
            return k;
        };

        float err_tol = 0.00001f;
        /*
        vec3 h = vec3(
                (b[0] * g[1] - b[1] * g[0]) / (c[0] * g[1] - c[1] * g[0]),
                (b[0] * g[2] - b[2] * g[0]) / (c[0] * g[2] - c[2] * g[0]),
                (b[1] * g[2] - b[2] * g[1]) / (c[1] * g[2] - c[2] * g[1])
        );
        */

        vec3 h = vec3(
                -(((b.y/g.y) - (b.x/g.x)) /((c.x/g.x) - (c.y / g.y))),
                -(((b.z/g.z) - (b.x/g.x)) /((c.x/g.x) - (c.z / g.z))),
                -(((b.y/g.y) - (b.z/g.z)) /((c.z/g.z) - (c.y / g.y)))
        );
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                float k = solve_k(h[i], b[j], c[j], g[j]);
                vec3 RHS = solve_KH_RHS(k, h[i], g, c);

                cout << "RHS: ";
                ifc::printVec3(RHS);
                cout << "b: ";
                ifc::printVec3(b);
                cout << endl;

                if(ifc::equal(RHS, b, err_tol)){
                    cout << "^ Correct" << endl;
                    return vec2(k, h[i]);
                }
            }
        }
        cout << "No Solution found" << endl;
        return vec2(-9999, -9999);
    };

    vec2 kh0 = solve_KH(b0, g0, c0);
    vec2 kh1 = solve_KH(b3, g2, c2);

    float k0 = kh0.x;
    float h0 = kh0.y;
    float k1 = kh1.x;
    float h1 = kh1.y;

    auto solveD = [k0, h0, k1, h1, solve_g, solve_c](float v){

        auto solveK = [k0, k1](float v){
            float k = k0 * (1.0f - v) + k1*v;
            return k;
        };
        auto solveH = [h0, h1](float v){
            float h = h0 * (1.0f - v) + h1*v;
            return h;
        };

        float k = solveK(v);
        float h = solveH(v);
        vec3 g = solve_g(v);
        vec3 c = solve_c(v);

        float x = (g.x * k) + (c.x * h);
        float y = (g.y * k) + (c.y * h);
        float z = (g.z * k) + (c.z * h);

        return vec3(x,y,z);
    };

    vector<vec3> D = {solveD(0.0f), solveD(0.333f),
                      solveD(0.666f), solveD(1.0f)};

    fillingData.DLeft = D;
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
        vec3 a0 = fillingData[j].a0PointLeft;
        vec3 a3 = fillingData[j].a3PointLeft;
        vec3 b3 = fillingData[j].b3PointLeft;

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

        // g0, g1, g2
        vec4 C0 = bezierSpline->computeVec4(0.0f);
        vec4 C05 = bezierSpline->computeVec4(0.5f);
        vec4 C1 = bezierSpline->computeVec4(1.0f);

        vec3 g0 = fillingData[j].g0PointLeft;
        vec3 g1 = fillingData[j].g1PointLeft;
        vec3 g2 = fillingData[j].g2PointLeft;

        vec4 g04 = vec4(g0.x, g0.y, g0.z, 1.0f);
        vec4 g14 = vec4(g1.x, g1.y, g1.z, 1.0f);
        vec4 g24 = vec4(g2.x, g2.y, g2.z, 1.0f);

        Line* g0Line = objectFactory.createLine("line", C0, g04);
        Line* g1Line = objectFactory.createLine("line", C05, g14);
        Line* g2Line = objectFactory.createLine("line", C1, g24);
        g0Line->setColor(debugColors.GGieldColor);
        g1Line->setColor(debugColors.GGieldColor);
        g2Line->setColor(debugColors.GGieldColor);

        // Di
        vec3 d0 = fillingData[j].DLeft[0];
        vec3 d1 = fillingData[j].DLeft[1];
        vec3 d2 = fillingData[j].DLeft[2];
        vec3 d3 = fillingData[j].DLeft[3];

        Line* d0Line = objectFactory.createLine("line", P0->getPosition(), d0);
        Line* d1Line = objectFactory.createLine("line", P1->getPosition(), d1);
        Line* d2Line = objectFactory.createLine("line", P2->getPosition(), d2);
        Line* d3Line = objectFactory.createLine("line", P3->getPosition(), d3);
        d0Line->setLineWidth(2.0f);
        d1Line->setLineWidth(2.0f);
        d2Line->setLineWidth(2.0f);
        d3Line->setLineWidth(2.0f);

        // Top g0, g1, g2
        vec3 C0Top = cubicBernstein3(
                fillingData[j].left->P0_BorderCurveMidPoint,
                fillingData[j].left->P1_Tanget,
                fillingData[j].left->P2,
                fillingData[j].left->P3_Center, 0.0f);
        vec3 C05Top = cubicBernstein3(
                fillingData[j].left->P0_BorderCurveMidPoint,
                fillingData[j].left->P1_Tanget,
                fillingData[j].left->P2,
                fillingData[j].left->P3_Center, 0.5f);
        vec3 C1Top = cubicBernstein3(
                fillingData[j].left->P0_BorderCurveMidPoint,
                fillingData[j].left->P1_Tanget,
                fillingData[j].left->P2,
                fillingData[j].left->P3_Center, 1.0f);
        vec3 g0Top = fillingData[j].g0PointTop;
        vec3 g1Top = fillingData[j].g1PointTop;
        vec3 g2Top = fillingData[j].g2PointTop;

        Line* g0TopLine = objectFactory.createLine("line", C0Top, g0Top);
        Line* g1TopLine = objectFactory.createLine("line", C05Top, g1Top);
        Line* g2TopLine = objectFactory.createLine("line", C1Top, g2Top);
        g0TopLine->setColor(debugColors.GGieldColor);
        g1TopLine->setColor(debugColors.GGieldColor);
        g2TopLine->setColor(debugColors.GGieldColor);

        scene->addRenderObject(P0);
        scene->addRenderObject(P1);
        scene->addRenderObject(P2);
        scene->addRenderObject(P3);
        scene->addRenderObject(tangetLine);

        if(j == 0){
            scene->addRenderObject(a0Line);
            scene->addRenderObject(a3Line);
            scene->addRenderObject(b3Line);
/*
            scene->addRenderObject(g0Line);
            scene->addRenderObject(g1Line);
            scene->addRenderObject(g2Line);
*/
            scene->addRenderObject(g0TopLine);
            scene->addRenderObject(g1TopLine);
            scene->addRenderObject(g2TopLine);
/*
            scene->addRenderObject(d0Line);
            scene->addRenderObject(d1Line);
            scene->addRenderObject(d2Line);
            scene->addRenderObject(d3Line);
*/
            fillingData[j].left->surface->setColor(Color(1.0f, 0.0, 0.0));
            fillingData[j].right->surface->setColor(Color(0.0f, 0.0, 1.0));
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
