//
// Created by jakub on 6/25/16.
//

#include <infinity_cad/geometry/filling/surface_filling.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/math/math.h>
#include <infinity_cad/geometry/polynomials/bernstein_basis.h>

using namespace std;
using namespace glm;

SurfaceFilling::SurfaceFilling(SceneID id, std::string name,
                               Surface* surface1,
                               Surface* surface2,
                               Surface* surface3) :
        RenderObject(id, name),
        fillingStatus(FillingStatus::COMPUTING){
    fillingData[0].surface = surface1;
    fillingData[1].surface = surface2;
    fillingData[2].surface = surface3;

    renderMode = RenderMode::RENDER_ALL;

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

    debugColors.HalfBezierControlPointColor = Color(1.0f, 1.0f, 1.0f);

    doRenderDebug = false;

    TANGET_MULTIPLIER = 0.50;
    //TANGET_MULTIPLIER = 1.0;
}

SurfaceFilling::~SurfaceFilling() {

}

//-----------------------//
//  PRIVATE
//-----------------------//

void SurfaceFilling::findCommonPoints(){
    counter = 0;
    CommonPointResult result1 = findCommonPoint(fillingData[0].surface,
                                                fillingData[1].surface);
    ifc::Point* point12 = result1.point;

    CommonPointResult result2 = findCommonPoint(fillingData[0].surface,
                                                fillingData[2].surface);
    ifc::Point* point13 = result2.point;

    CommonPointResult result3 = findCommonPoint(fillingData[1].surface,
                                                fillingData[2].surface);
    ifc::Point* point23 = result3.point;

    // --------------



    // --------------

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

CommonPointResult SurfaceFilling::findCommonPoint(Surface* surface1,
                                                  Surface* surface2){
    CommonPointResult result;

    const vector<ifc::Point*>& points1 = surface1->getAllPoints();
    const vector<ifc::Point*>& points2 = surface2->getAllPoints();

    for(unsigned int i = 0; i < points1.size(); i++){
        for(unsigned int j = 0; j < points2.size(); j++){
            if(points1[i] == points2[j]){
                result.point = points1[i];
                PointIndex point_index1 = surface1->getIndex(result.point);
                PointIndex point_index2 = surface2->getIndex(result.point);

                result.position1 = determinePointPosition(point_index1);
                result.position2 = determinePointPosition(point_index2);

                std::cout << counter << std::endl;
                std::cout
                << commonPointPositionToString(result.position1)
                << std::endl;
                std::cout
                << commonPointPositionToString(result.position2)
                << std::endl;

                std::cout
                << pointIndexToString(point_index1)
                << std::endl;
                std::cout
                << pointIndexToString(point_index2)
                << std::endl;
                std::cout << std::endl;
                counter++;
            }
        }
    }
    return result;
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

    computeBorderCurveSideType(fillingData[0]);
    computeBorderCurveSideType(fillingData[1]);
    computeBorderCurveSideType(fillingData[2]);

    CommonPointResult result1 = findCommonPoint(fillingData[0].surface,
                                                fillingData[1].surface);
    CommonPointResult result2 = findCommonPoint(fillingData[0].surface,
                                                fillingData[2].surface);
    CommonPointResult result3 = findCommonPoint(fillingData[1].surface,
                                                fillingData[2].surface);

    fillingData[0].half_bezier_which_side =
            determineHalfBezierSide(fillingData[0].borderCurveSide,
                                    result1.position1);
    fillingData[0].half_bezier_which_side_top =
            determineHalfBezierSide(fillingData[0].left->borderCurveSide,
                                    result1.position2);

    fillingData[0].left->half_bezier_which_side =
            determineHalfBezierOpositeSide(fillingData[0].left->borderCurveSide,
                                           result1.position2);
    fillingData[0].left->half_bezier_which_side_top =
            determineHalfBezierSide(
                    fillingData[0].left->left->borderCurveSide,
                    result3.position2);

    fillingData[0].left->left->half_bezier_which_side =
            determineHalfBezierOpositeSide(
                    fillingData[0].left->left->borderCurveSide,
                    result3.position2);
    fillingData[0].left->left->half_bezier_which_side_top =
            determineHalfBezierOpositeSide(
                    fillingData[0].borderCurveSide,
                    result1.position1);

    std::cout << "0" << std::endl
    << "Base: "
    << halfBezierWhichSideToString(fillingData[0].half_bezier_which_side)
    << std::endl << "Top:  "
    << halfBezierWhichSideToString(fillingData[0].half_bezier_which_side_top)
    << std::endl << std::endl;

    std::cout << "1" << std::endl
    << "Base: "
    << halfBezierWhichSideToString(fillingData[1].half_bezier_which_side)
    << std::endl << "Top:  "
    << halfBezierWhichSideToString(fillingData[1].half_bezier_which_side_top)
    << std::endl << std::endl;

    std::cout << "2" << std::endl
    << "Base: "
    << halfBezierWhichSideToString(fillingData[2].half_bezier_which_side)
    << std::endl << "Top:  "
    << halfBezierWhichSideToString(fillingData[2].half_bezier_which_side_top)
    << std::endl << std::endl;

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

void SurfaceFilling::computeBorderCurveSideType(FillingData &fillingData){
    if(fillingData.constBorderCurveParam == BorderCurveParam::U1 &&
       fillingData.varBorderCurveParam == BorderCurveParam:: V){
        fillingData.borderCurveSide = BorderCurveSide::RIGHT;
    }
    else if(fillingData.constBorderCurveParam == BorderCurveParam::U0 &&
            fillingData.varBorderCurveParam == BorderCurveParam:: V){
        fillingData.borderCurveSide = BorderCurveSide::LEFT;
    }
    else if(fillingData.constBorderCurveParam == BorderCurveParam::V0 &&
            fillingData.varBorderCurveParam == BorderCurveParam:: U){
        fillingData.borderCurveSide = BorderCurveSide::BOTTOM;
    }
    else if(fillingData.constBorderCurveParam == BorderCurveParam::V1 &&
            fillingData.varBorderCurveParam == BorderCurveParam:: U){
        fillingData.borderCurveSide = BorderCurveSide::TOP;
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

    //tangent = normalize(tangent);
    tangent *= 0.33f;
    tangent *= TANGET_MULTIPLIER;

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

    computeDTop(fillingData[0]);
    computeDTop(fillingData[1]);
    computeDTop(fillingData[2]);
}

void SurfaceFilling::computeGFieldVector(FillingData& fillingData){
    vec3 a0, b0, a3, b3;
    vec3 g0, g1, g2;

    vec2 uv = getUV(fillingData);
    //uv = vec2(0.0f, 0.5f);

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
    //a0 = normalize(a0);
    a0 *= 0.33f * TANGET_MULTIPLIER;

    b0 = a0;

    a3 = fillingData.left->P3_Center - fillingData.left->P2;
    b3 = fillingData.right->P3_Center - fillingData.right->P2;

    a3 = -a3;
    //b3 = -b3;

    //a3 = normalize(a3);
    a3 *= 0.33f * TANGET_MULTIPLIER;
    //b3 = normalize(b3);
    b3 *= 0.33f * TANGET_MULTIPLIER;

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
    fillingData.b3PointLeft = fillingData.P3_Center + b3;

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
    //uv = vec2(0.5f, 1.0f);

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
    //a0 = normalize(a0);
    a0 *= 0.33f * TANGET_MULTIPLIER;
    b0 = a0;

    a3 = fillingData.left->P3_Center - fillingData.left->P2;
    b3 = fillingData.right->P3_Center - fillingData.right->P2;

    a3 = -a3;
    //b3 = -b3;

    //a3 = normalize(a3);
    a3 *= 0.33f * TANGET_MULTIPLIER;
    //b3 = normalize(b3);
    b3 *= 0.33f * TANGET_MULTIPLIER;

    g0 = (a0 + b0) / 2.0f;
    g2 = (a3 + b3) / 2.0f;
    g1 = (g0 + g2) / 2.0f;

    g0 = -g0;
    g1 = -g1;
    g2 = -g2;

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

    fillingDataBase.g0PointTop = C0 + g0;
    fillingDataBase.g1PointTop = C05 + g1;
    fillingDataBase.g2PointTop = C1 + g2;

    fillingDataBase.a0PointTop = fillingData.P0_BorderCurveMidPoint + a0;
    fillingDataBase.b0PointTop = fillingData.P0_BorderCurveMidPoint + b0;
    fillingDataBase.a3PointTop = fillingData.P3_Center + a3;
    fillingDataBase.b3PointTop = fillingData.P3_Center + b3;

    delete P0;
    delete P1;
    delete P2;
    delete P3;
    delete bezierSpline;
}

void SurfaceFilling::computeD(FillingData &fillingData){
    vec3 b0 = fillingData.b0PointLeft;
    vec3 b3 = fillingData.b3PointLeft;

    vec3 g0 = fillingData.g0PointLeft;
    vec3 g2 = fillingData.g2PointLeft;

    vec3 c0 = fillingData.P1_Tanget - fillingData.P0_BorderCurveMidPoint;
    vec3 c1 = fillingData.P2 - fillingData.P1_Tanget;
    vec3 c2 = fillingData.P3_Center - fillingData.P2;
/*
    auto B2 = [](float b0, float b1, float b2, float t){
        float val = b0 - t * (2 * b0 - 2 * b1 - t * (b0 - 2 * b1 + b2));
        return val;
    };
    auto solve_c = [c0, c1, c2, B2](float t){
        float x = B2(c0.x, c1.x, c2.x, t);
        float y = B2(c0.y, c1.y, c2.y, t);
        float z = B2(c0.z, c1.z, c2.z, t);

        return vec3(x,y,z);
    };

    auto solve_g = [fillingData, B2](float t){
        vec3& g0 = fillingData.g0T
        float x = B2(c0.x, c1.x, c2.x, t);
        float y = B2(c0.y, c1.y, c2.y, t);
        float z = B2(c0.z, c1.z, c2.z, t);

        return vec3(x,y,z);
    };*/

    auto solve_c = [c0, c1, c2](float t){
        vec3 v = quadraticBernstein3(c0, c1, c2, t);
        return v;
    };
    auto solve_g = [fillingData](float t){
        vec3 v = quadraticBernstein3(fillingData.g0TangentLeft,
                                     fillingData.g1TangentLeft,
                                     fillingData.g2TangentLeft, t);
        return v;
    };

    auto solve_KH_RHS = [](float k, float h, vec3& gg, vec3& cc){
        float x = (k * gg.x) + (h * cc.x);
        float y = (k * gg.y) + (h * cc.y);
        float z = (k * gg.z) + (h * cc.z);

        return vec3(x,y,z);
    };

    float k0 = (((c0.x*b0.y) - (b0.x*c0.y)) / ((c0.x*g0.y) - (g0.x*c0.y)));
    float h0 = (((b0.x*g0.y) - (b0.y*g0.x)) / ((c0.x*g0.y) - (g0.x*c0.y)));
    float k1 = (((c2.x*b3.y) - (b3.x*c2.y)) / ((c2.x*g2.y) - (g2.x*c2.y)));
    float h1 = (((b3.x*g2.y) - (b3.y*g2.x)) / ((c2.x*g2.y) - (g2.x*c2.y)));

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

    vec3 d0 = solveD(0.0f);
    vec3 d1 = solveD(0.333f);
    vec3 d2 = solveD(0.666f);
    vec3 d3 = solveD(1.0f);

    if(fillingData.half_bezier_which_side == HalfBezierWhichSide::V_SECOND
       || fillingData.half_bezier_which_side == HalfBezierWhichSide::U_FIRST){
        d0 = -d0;
        d1 = -d1;
        d2 = -d2;
        d3 = -d3;
    }

    vector<vec3> D = {fillingData.P0_BorderCurveMidPoint + d0,
                      fillingData.P1_Tanget + d1,
                      fillingData.P2 + d2,
                      fillingData.P3_Center + d3};

    fillingData.DLeft = D;
}

void SurfaceFilling::computeDTop(FillingData &fillingDataBase){
    FillingData& fillingData = *(fillingDataBase.left);

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
        vec3 v = quadraticBernstein3(fillingData.g0TangentLeft,
                                     fillingData.g1TangentLeft,
                                     fillingData.g2TangentLeft, t);
        return v;
    };

    auto solve_KH_RHS = [](float k, float h, vec3& gg, vec3& cc){
        float x = (k * gg.x) + (h * cc.x);
        float y = (k * gg.y) + (h * cc.y);
        float z = (k * gg.z) + (h * cc.z);

        return vec3(x,y,z);
    };

    float k0 = (((c0.x*b0.y) - (b0.x*c0.y)) / ((c0.x*g0.y) - (g0.x*c0.y)));
    float h0 = (((b0.x*g0.y) - (b0.y*g0.x)) / ((c0.x*g0.y) - (g0.x*c0.y)));
    float k1 = (((c2.x*b3.y) - (b3.x*c2.y)) / ((c2.x*g2.y) - (g2.x*c2.y)));
    float h1 = (((b3.x*g2.y) - (b3.y*g2.x)) / ((c2.x*g2.y) - (g2.x*c2.y)));

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

    vec3 d0 = solveD(0.0f);
    vec3 d1 = solveD(0.333f);
    vec3 d2 = solveD(0.666f);
    vec3 d3 = solveD(1.0f);

    if(fillingDataBase.half_bezier_which_side_top ==
            HalfBezierWhichSide::V_FIRST
            || fillingDataBase.half_bezier_which_side_top ==
               HalfBezierWhichSide::U_SECOND){
        d0 = -d0;
        d1 = -d1;
        d2 = -d2;
        d3 = -d3;
    }

    vector<vec3> D = {fillingData.P0_BorderCurveMidPoint - d0,
                      fillingData.P1_Tanget - d1,
                      fillingData.P2 - d2,
                      fillingData.P3_Center - d3};


    fillingDataBase.DTop = D;
}

void SurfaceFilling::computeHalfBorderCurvePoints(){
    computeHalfBorderCurvePoints(fillingData[0]);
    computeHalfBorderCurvePoints(fillingData[1]);
    computeHalfBorderCurvePoints(fillingData[2]);

    computeHalfBorderCurvePointsTop(fillingData[0]);
    computeHalfBorderCurvePointsTop(fillingData[1]);
    computeHalfBorderCurvePointsTop(fillingData[2]);
}

void SurfaceFilling::computeHalfBorderCurvePoints(FillingData& fillingData){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    BezierSplineC0* bezier
            = objectFactory.createBezier("bezier", fillingData.borderPoints);

    const HalfBorderCurveData data
            = getHalfBezierCurveDataBasedOnSide(
                    fillingData.half_bezier_which_side);

    float start = data.start;
    float end = data.end;
    float p = data.p;
    float k = data.k;

    vec3 p0 = bezier->compute(start);
    vec3 p1 = bezier->compute(p);
    vec3 p2 = bezier->compute(k);
    vec3 p3 = bezier->compute(end);

    vector<vec3> halfBezier = CalculateBezierControlPoints(p0, p1, p2, p3,
                                                           1.0f/3.0f,
                                                           2.0f/3.0f);

    fillingData.halfBezierPointsPointsBase = halfBezier;

    vec2 uv = getUV(fillingData);
    vec3 tangent1, tangent2;

    if(fillingData.varBorderCurveParam == BorderCurveParam::U){
        tangent1 = fillingData.surface->computeDv(p, uv.y);
        tangent2 = fillingData.surface->computeDv(k, uv.y);
    }
    else if(fillingData.varBorderCurveParam == BorderCurveParam::V){
        tangent1 = fillingData.surface->computeDu(uv.x, p);
        tangent2 = fillingData.surface->computeDu(uv.x, k);
    }
    if(fillingData.constBorderCurveParam == BorderCurveParam::V0 ||
       fillingData.constBorderCurveParam == BorderCurveParam::U0){
        tangent1 = -tangent1;
        tangent2 = -tangent2;
    }

    tangent1 *= 0.33f * TANGET_MULTIPLIER;
    tangent2 *= 0.33f * TANGET_MULTIPLIER;

    fillingData.halfBezierTanget1Base = tangent1;
    fillingData.halfBezierTanget2Base = tangent2;

    fillingData.halfBezierPoint1Base = p1 + tangent1;
    fillingData.halfBezierPoint2Base = p2 + tangent2;
    fillingData.halfBezierPoint1BaseOrigin = p1;
    fillingData.halfBezierPoint2BaseOrigin = p2;
    delete bezier;
}

void SurfaceFilling::computeHalfBorderCurvePointsTop(
        FillingData& fillingDataBase){
    FillingData& fillingData = *(fillingDataBase.left);

    const HalfBorderCurveData data
            = getHalfBezierCurveDataBasedOnSide(
                    fillingDataBase.half_bezier_which_side_top);

    float start = data.start;
    float end = data.end;
    float p = data.p;
    float k = data.k;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    BezierSplineC0* bezier
            = objectFactory.createBezier("bezier", fillingData.borderPoints);

    vec3 p0 = bezier->compute(start);
    vec3 p1 = bezier->compute(p);
    vec3 p2 = bezier->compute(k);
    vec3 p3 = bezier->compute(end);

    vector<vec3> halfBezier = CalculateBezierControlPoints(p0, p1, p2, p3,
                                                           1.0f/3.0f,
                                                           2.0f/3.0f);

    vec2 uv = getUV(fillingData);
    vec3 tangent1, tangent2;

    if(fillingData.varBorderCurveParam == BorderCurveParam::U){
        tangent1 = fillingData.surface->computeDv(p, uv.y);
        tangent2 = fillingData.surface->computeDv(k, uv.y);
    }
    else if(fillingData.varBorderCurveParam == BorderCurveParam::V){
        tangent1 = fillingData.surface->computeDu(uv.x, p);
        tangent2 = fillingData.surface->computeDu(uv.x, k);
    }
    if(fillingData.constBorderCurveParam == BorderCurveParam::V0 ||
       fillingData.constBorderCurveParam == BorderCurveParam::U0){
        tangent1 = -tangent1;
        tangent2 = -tangent2;
    }

    tangent1 *= 0.33f * TANGET_MULTIPLIER;
    tangent2 *= 0.33f * TANGET_MULTIPLIER;

    fillingDataBase.halfBezierPointsPointsTop = halfBezier;
    fillingDataBase.halfBezierTanget1Top = tangent1;
    fillingDataBase.halfBezierTanget2Top = tangent2;

    fillingDataBase.halfBezierPoint1Top = p1 + tangent1;
    fillingDataBase.halfBezierPoint2Top = p2 + tangent2;
    fillingData.halfBezierPoint1TopOrigin = p1;
    fillingData.halfBezierPoint2TopOrigin = p2;
    delete bezier;

    //if()
}

vector<vec3> SurfaceFilling::CalculateBezierControlPoints(vec3 p0, vec3 p1,
                                                          vec3 p2, vec3 p3,
                                                          float u, float v) {
    vector<vec3> pos(4);
    float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f, det = 0.0f;
    vec3 q1, q2;

    if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0) || (u >= v)){
        cout << "Failure" << endl;
        return vector<vec3>();
    }

    a = 3 * (1 - u) * (1 - u) * u; b = 3 * (1 - u) * u * u;
    c = 3 * (1 - v) * (1 - v) * v; d = 3 * (1 - v) * v * v;
    det = a * d - b * c;
    if (det == 0.0) {
        cout << "Failure" << endl;
        return vector<vec3>();
    }

    pos[0].x = p0.x; pos[0].y = p0.y; pos[0].z = p0.z;
    pos[3].x = p3.x; pos[3].y = p3.y; pos[3].z = p3.z;

    q1.x = p1.x - ((1 - u) * (1 - u) * (1 - u) * p0.x + u * u * u * p3.x);
    q1.y = p1.y - ((1 - u) * (1 - u) * (1 - u) * p0.y + u * u * u * p3.y);
    q1.z = p1.z - ((1 - u) * (1 - u) * (1 - u) * p0.z + u * u * u * p3.z);

    q2.x = p2.x - ((1 - v) * (1 - v) * (1 - v) * p0.x + v * v * v * p3.x);
    q2.y = p2.y - ((1 - v) * (1 - v) * (1 - v) * p0.y + v * v * v * p3.y);
    q2.z = p2.z - ((1 - v) * (1 - v) * (1 - v) * p0.z + v * v * v * p3.z);

    pos[1].x = d * q1.x - b * q2.x;
    pos[1].y = d * q1.y - b * q2.y;
    pos[1].z = d * q1.z - b * q2.z;
    pos[1].x /= det;
    pos[1].y /= det;
    pos[1].z /= det;

    pos[2].x = (-c) * q1.x + a * q2.x;
    pos[2].y = (-c) * q1.y + a * q2.y;
    pos[2].z = (-c) * q1.z + a * q2.z;
    pos[2].x /= det;
    pos[2].y /= det;
    pos[2].z /= det;

    return pos;
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

/*
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
*/
std::string SurfaceFilling::borderCurveParamToString(BorderCurveParam param){
    if(param == BorderCurveParam::U) return "U";
    if(param == BorderCurveParam::V) return "V";
    if(param == BorderCurveParam::U0) return "U0";
    if(param == BorderCurveParam::U1) return "U1";
    if(param == BorderCurveParam::V0) return "V0";
    if(param == BorderCurveParam::V1) return "V1";
    return "Unknown";
}

std::string SurfaceFilling::borderCurveSideToString(BorderCurveSide param){
    if(param == BorderCurveSide::TOP) return "TOP";
    if(param == BorderCurveSide::BOTTOM) return "BOTTOM";
    if(param == BorderCurveSide::LEFT) return "LEFT";
    if(param == BorderCurveSide::RIGHT) return "RIGHT";
    return "Unknown";
};

std::string SurfaceFilling::pointSurfaceTypeToString(ifc::PointSurfaceType param){
    if(param == ifc::PointSurfaceType::U0V1) return "U0V1";
    if(param == ifc::PointSurfaceType::U0V0) return "U0V0";
    if(param == ifc::PointSurfaceType::U1V1) return "U1V1";
    if(param == ifc::PointSurfaceType::U1V0) return "U1V0";
    if(param == ifc::PointSurfaceType::NONE) return "NONE";
    return "Unknown";
};

std::string SurfaceFilling::pointIndexToString(PointIndex point_index){
    std::string str = "[" + std::to_string(point_index.row_index) +
            ", " + std::to_string(point_index.column_index) + "]";
    return str;
};

std::string SurfaceFilling::commonPointPositionToString(
        CommonPointPosition position){
    if(position == CommonPointPosition::BOTTOM_LEFT)
        return "BOTTOM_LEFT";
    if(position == CommonPointPosition::BOTTOM_RIGHT)
        return "BOTTOM_RIGHT";
    if(position == CommonPointPosition::TOP_RIGHT)
        return "TOP_RIGHT";
    if(position == CommonPointPosition::TOP_LEFT)
        return "TOP_LEFT";
}

std::string SurfaceFilling::halfBezierWhichSideToString(
        HalfBezierWhichSide side){
    if(side == HalfBezierWhichSide::U_FIRST)
        return "U_FIRST";
    if(side == HalfBezierWhichSide::U_SECOND)
        return "U_SECOND";
    if(side == HalfBezierWhichSide::V_FIRST)
        return "V_FIRST";
    if(side == HalfBezierWhichSide::V_SECOND)
        return "V_SECOND";
    return "Unknown";
}

CommonPointPosition SurfaceFilling::determinePointPosition(PointIndex index){
    if(index.row_index == 0 && index.column_index == 0)
        return CommonPointPosition::BOTTOM_LEFT;
    if(index.row_index == 3 && index.column_index == 0)
        return CommonPointPosition::TOP_LEFT;
    if(index.row_index == 0 && index.column_index == 3)
        return CommonPointPosition::BOTTOM_RIGHT;
    if(index.row_index == 3 && index.column_index == 3)
        return CommonPointPosition::TOP_RIGHT;
}

HalfBezierWhichSide SurfaceFilling::determineHalfBezierSide(
        BorderCurveSide border_curve_side,
        CommonPointPosition common_point_position){
    if(border_curve_side == BorderCurveSide::LEFT &&
            common_point_position == CommonPointPosition::BOTTOM_LEFT){
        return HalfBezierWhichSide::U_FIRST;
    }
    if(border_curve_side == BorderCurveSide::LEFT &&
       common_point_position == CommonPointPosition::TOP_LEFT){
        return HalfBezierWhichSide::U_SECOND;
    }

    if(border_curve_side == BorderCurveSide::RIGHT &&
       common_point_position == CommonPointPosition::BOTTOM_RIGHT){
        return HalfBezierWhichSide::U_FIRST;
    }
    if(border_curve_side == BorderCurveSide::RIGHT &&
       common_point_position == CommonPointPosition::TOP_RIGHT){
        return HalfBezierWhichSide::U_SECOND;
    }

    if(border_curve_side == BorderCurveSide::TOP &&
       common_point_position == CommonPointPosition::TOP_RIGHT){
        return HalfBezierWhichSide::V_SECOND;
    }
    if(border_curve_side == BorderCurveSide::TOP &&
       common_point_position == CommonPointPosition::TOP_LEFT){
        return HalfBezierWhichSide::V_FIRST;
    }
    if(border_curve_side == BorderCurveSide::BOTTOM &&
       common_point_position == CommonPointPosition::BOTTOM_RIGHT){
        return HalfBezierWhichSide::V_SECOND;
    }
    if(border_curve_side == BorderCurveSide::BOTTOM &&
       common_point_position == CommonPointPosition::BOTTOM_LEFT){
        return HalfBezierWhichSide::V_FIRST;
    }
}

HalfBezierWhichSide SurfaceFilling::determineHalfBezierOpositeSide(
        BorderCurveSide border_curve_side,
        CommonPointPosition common_point_position){
    if(border_curve_side == BorderCurveSide::LEFT &&
       common_point_position == CommonPointPosition::BOTTOM_LEFT){
        return HalfBezierWhichSide::U_SECOND;
    }
    if(border_curve_side == BorderCurveSide::LEFT &&
       common_point_position == CommonPointPosition::TOP_LEFT){
        return HalfBezierWhichSide::U_FIRST;
    }

    if(border_curve_side == BorderCurveSide::RIGHT &&
       common_point_position == CommonPointPosition::BOTTOM_RIGHT){
        return HalfBezierWhichSide::U_SECOND;
    }
    if(border_curve_side == BorderCurveSide::RIGHT &&
       common_point_position == CommonPointPosition::TOP_RIGHT){
        return HalfBezierWhichSide::U_FIRST;
    }

    if(border_curve_side == BorderCurveSide::TOP &&
       common_point_position == CommonPointPosition::TOP_RIGHT){
        return HalfBezierWhichSide::V_FIRST;
    }
    if(border_curve_side == BorderCurveSide::TOP &&
       common_point_position == CommonPointPosition::TOP_LEFT){
        return HalfBezierWhichSide::V_SECOND;
    }

    if(border_curve_side == BorderCurveSide::BOTTOM &&
       common_point_position == CommonPointPosition::BOTTOM_RIGHT){
        return HalfBezierWhichSide::V_FIRST;
    }
    if(border_curve_side == BorderCurveSide::BOTTOM &&
       common_point_position == CommonPointPosition::BOTTOM_LEFT){
        return HalfBezierWhichSide::V_SECOND;
    }
}

const HalfBorderCurveData SurfaceFilling::getHalfBezierCurveDataBasedOnSide(
        HalfBezierWhichSide side){
    if(side == HalfBezierWhichSide::U_FIRST)
        return U_FIRST_DATA;
    if(side == HalfBezierWhichSide::U_SECOND)
        return U_SECOND_DATA;
    if(side == HalfBezierWhichSide::V_FIRST)
        return V_SECOND_DATA;
    if(side == HalfBezierWhichSide::V_SECOND)
        return V_FIRST_DATA;
}

void SurfaceFilling::renderPatches(const glm::mat4 &VP){
    if(renderMode == RenderMode::RENDER_ALL){
        renderPatch(VP, fillingData[0]);
        renderPatch(VP, fillingData[1]);
        renderPatch(VP, fillingData[2]);
    }
    else if (renderMode == RenderMode::RENDER_FIRST_PATCH){
        renderPatch(VP, fillingData[0]);
    }
    else if (renderMode == RenderMode::RENDER_SECOND_PATCH){
        renderPatch(VP, fillingData[1]);
    }
    else if (renderMode == RenderMode::RENDER_THIRD_PATCH){
        renderPatch(VP, fillingData[2]);
    }

}

void SurfaceFilling::renderPatch(const glm::mat4 &VP, FillingData& fillingData){
    vec3 P0 = fillingData.halfBezierPointsPointsBase[0];
    vec3 e0_p = fillingData.halfBezierPointsPointsBase[1];
    vec3 e1_m = fillingData.halfBezierPointsPointsBase[2];
    vec3 P1 = fillingData.halfBezierPointsPointsBase[3];

    if(fillingData.half_bezier_which_side == HalfBezierWhichSide::U_SECOND ||
       fillingData.half_bezier_which_side == HalfBezierWhichSide::V_SECOND) {
        P0 = fillingData.halfBezierPointsPointsBase[3];
        e0_p = fillingData.halfBezierPointsPointsBase[2];
        e1_m = fillingData.halfBezierPointsPointsBase[1];
        P1 = fillingData.halfBezierPointsPointsBase[0];
    }

    vec3 e0_m = fillingData.halfBezierPointsPointsTop[1];
    vec3 f0_m = fillingData.halfBezierPoint1Top;
    vec3 f0_p = fillingData.halfBezierPoint1Base;
    vec3 f1_m = fillingData.halfBezierPoint2Base;
    vec3 f1_p = fillingData.DLeft[1];
    //vec3 f1_p = fillingData.right->DLeft[2];

    vec3 e1_p = fillingData.P1_Tanget;
    vec3 e3_p = fillingData.halfBezierPointsPointsTop[2];

    if(fillingData.half_bezier_which_side_top == HalfBezierWhichSide::U_SECOND ||
       fillingData.half_bezier_which_side_top == HalfBezierWhichSide::V_SECOND) {
        e3_p = fillingData.halfBezierPointsPointsTop[1];
        e0_m = fillingData.halfBezierPointsPointsTop[2];
    }

    vec3 f3_p = fillingData.halfBezierPoint2Top;

    vec3 f3_m = fillingData.DTop[1];
    vec3 f2_p = fillingData.DTop[2];
    //vec3 f3_m = fillingData.right->DTop[2];
    //vec3 f2_p = fillingData.right->DTop[1];

    vec3 f2_m = fillingData.DLeft[2];
    //vec3 f2_m = fillingData.right->DLeft[1];

    vec3 e2_m = fillingData.P2;

    vec3 e3_m = fillingData.left->P1_Tanget;
    vec3 e2_p = fillingData.left->P2;
    vec3 P3 = fillingData.left->P0_BorderCurveMidPoint;
    vec3 P2 = fillingData.left->P3_Center;

    auto solveF0 = [f0_p, f0_m](float u, float v){
        float denom = u + v;
        if(denom == 0){
            return (f0_p + f0_m) / 2.0f;
        }
        vec3 f = (f0_p * u + f0_m * v) / denom;
        return f;
    };
    auto solveF1 = [f1_m, f1_p](float u, float v){
        float denom = 1 - u + v;
        if(denom == 0){
            return (f1_m + f1_p) / 2.0f;
        }
        vec3 f = (f1_m * (1.0f - u) + f1_p * v) / denom;
        return f;
    };

    auto solveF2 = [f2_p, f2_m](float u, float v){
        float denom = 2 - u - v;
        if(denom == 0){
            return (f2_p + f2_m) / 2.0f;
        }
        vec3 f = (f2_p * (1.0f - u) + f2_m * (1.0f - v)) / denom;
        return f;
    };

    auto solveF3 = [f3_m, f3_p](float u, float v){
        float denom = 1 + u - v;
        if(denom == 0){
            return (f3_m + f3_p) / 2.0f;
        }
        vec3 f = (f3_m * u + f3_p * (1.0f - v)) / denom;
        return f;
    };

    // DEBUG

    ObjectFactory &objectFactory = ObjectFactory::getInstance();
    Color color0(1.0f, 0.0f, 0.0f); // red
    Color color1(1.0f, 1.0f, 0.0f); // yellow
    Color color2(1.0f, 0.0f, 1.0f); // pink
    Color color3(0.0f, 0.0f, 1.0f); // blue

    //ifc::Point* pointP0 = objectFactory.createPoint("p", P0);
    //ifc::Point* pointP1 = objectFactory.createPoint("p", P1);
    //ifc::Point* pointP0 = objectFactory.createPoint("p", f3_m);
    //ifc::Point* pointP1 = objectFactory.createPoint("p", f2_p);
    ifc::Point* pointP0 = objectFactory.createPoint("p", f3_p);
    ifc::Point* pointP1 = objectFactory.createPoint("p", f2_m);
    ifc::Point* pointP2 = objectFactory.createPoint("p", f3_m);
    ifc::Point* pointP3 = objectFactory.createPoint("p", f2_p);
    //ifc::Point* pointP2 = objectFactory.createPoint("p", P2);
    //ifc::Point* pointP3 = objectFactory.createPoint("p", P3);

    pointP0->update();
    pointP1->update();
    pointP2->update();
    pointP3->update();

    pointP0->setColor(color0);
    pointP1->setColor(color1);
    pointP2->setColor(color2);
    pointP3->setColor(color3);

    pointP0->render(VP);
    pointP1->render(VP);
    pointP2->render(VP);
    pointP3->render(VP);


    //

    mat4 Gx;
    mat4 Gy;
    mat4 Gz;

    Gx[0].x = P0.x;
    Gx[0].y = e0_p.x;
    Gx[0].z = e1_m.x;
    Gx[0].w = P1.x;

    Gx[1].x = e0_m.x;
    Gx[1].y = 0.0f; // F0
    Gx[1].z = 0.0f; // F1
    Gx[1].w = e1_p.x;

    Gx[2].x = e3_p.x;
    Gx[2].y = 0.0f; // F3
    Gx[2].z = 0.0f; // F2
    Gx[2].w = e2_m.x;

    Gx[3].x = P3.x;
    Gx[3].y = e3_m.x;
    Gx[3].z = e2_p.x;
    Gx[3].w = P2.x;

    Gy[0].x = P0.y;
    Gy[0].y = e0_p.y;
    Gy[0].z = e1_m.y;
    Gy[0].w = P1.y;

    Gy[1].x = e0_m.y;
    Gy[1].y = 0.0f; // F0
    Gy[1].z = 0.0f; // F1
    Gy[1].w = e1_p.y;

    Gy[2].x = e3_p.y;
    Gy[2].y = 0.0f; // F3
    Gy[2].z = 0.0f; // F2
    Gy[2].w = e2_m.y;

    Gy[3].x = P3.y;
    Gy[3].y = e3_m.y;
    Gy[3].z = e2_p.y;
    Gy[3].w = P2.y;

    Gz[0].x = P0.z;
    Gz[0].y = e0_p.z;
    Gz[0].z = e1_m.z;
    Gz[0].w = P1.z;

    Gz[1].x = e0_m.z;
    Gz[1].y = 0.0f; // F0
    Gz[1].z = 0.0f; // F1
    Gz[1].w = e1_p.z;

    Gz[2].x = e3_p.z;
    Gz[2].y = 0.0f; // F3
    Gz[2].z = 0.0f; // F2
    Gz[2].w = e2_m.z;

    Gz[3].x = P3.z;
    Gz[3].y = e3_m.z;
    Gz[3].z = e2_p.z;
    Gz[3].w = P2.z;

    float u_min = 0.0f;
    float u_max = 1.0f;
    float v_min = 0.0f;
    float v_max = 1.0f;
    float du = 0.008f;
    float dv = 0.008f;

    float du_tmp = du;
    float dv_tmp = dv;

    int uDivisionCount_tmp = Surface::uDivisionCount;
    if(uDivisionCount_tmp == 1) uDivisionCount_tmp++;

    int vDivisionCount_tmp = Surface::vDivisionCount;
    if(vDivisionCount_tmp == 1) vDivisionCount_tmp++;

    du = 1.0f / (float)(uDivisionCount_tmp - 1);

    vector<vec4> points;
    int index = 0;
    vector<vector<int>> edges;

    float u,v;
    for(u = u_min; u <= u_max; u+=du){
        for(v = v_min; v <= v_max; v+=dv){
            vec4 Bu = cubicBernsteinVector(u);
            vec4 Bv = cubicBernsteinVector(v);

            vec3 F0 = solveF0(u, v);
            vec3 F1 = solveF1(u, v);
            vec3 F2 = solveF2(u, v);
            vec3 F3 = solveF3(u, v);

            Gx[1].y = F0.x;
            Gx[1].z = F1.x;

            Gy[1].y = F0.y;
            Gy[1].z = F1.y;

            Gz[1].y = F0.z;
            Gz[1].z = F1.z;

            Gx[2].y = F3.x;
            Gx[2].z = F2.x;

            Gy[2].y = F3.y;
            Gy[2].z = F2.y;

            Gz[2].y = F3.z;
            Gz[2].z = F2.z;

            float x = ifc::getMultplicationValue(Bu, Gx, Bv);
            float y = ifc::getMultplicationValue(Bu, Gy, Bv);
            float z = ifc::getMultplicationValue(Bu, Gz, Bv);

            vec4 point(x, y, z, 1);
            point = VP * point;

            point.x /= point.w;
            point.y /= point.w;

            points.push_back(point);
            vector<int> edge{index, index+1};
            edges.push_back(edge);
            index++;
        }
        v = v_max;
        vec4 Bu = cubicBernsteinVector(u);
        vec4 Bv = cubicBernsteinVector(v);

        vec3 F0 = solveF0(u, v);
        vec3 F1 = solveF1(u, v);
        vec3 F2 = solveF2(u, v);
        vec3 F3 = solveF3(u, v);

        Gx[1].y = F0.x;
        Gx[1].z = F1.x;

        Gy[1].y = F0.y;
        Gy[1].z = F1.y;

        Gz[1].y = F0.z;
        Gz[1].z = F1.z;

        Gx[2].y = F3.x;
        Gx[2].z = F2.x;

        Gy[2].y = F3.y;
        Gy[2].z = F2.y;

        Gz[2].y = F3.z;
        Gz[2].z = F2.z;

        float x = ifc::getMultplicationValue(Bu, Gx, Bv);
        float y = ifc::getMultplicationValue(Bu, Gy, Bv);
        float z = ifc::getMultplicationValue(Bu, Gz, Bv);

        vec4 point(x, y, z, 1);
        point = VP * point;

        point.x /= point.w;
        point.y /= point.w;

        points.push_back(point);
        index++;
    }
    du = du_tmp;
    dv = 1.0f / (float)(vDivisionCount_tmp - 1);

    for(v = v_min; v <= v_max; v+=dv){
        for(u = u_min; u <= u_max; u+=du){
            vec4 Bu = cubicBernsteinVector(u);
            vec4 Bv = cubicBernsteinVector(v);

            vec3 F0 = solveF0(u, v);
            vec3 F1 = solveF1(u, v);
            vec3 F2 = solveF2(u, v);
            vec3 F3 = solveF3(u, v);

            Gx[1].y = F0.x;
            Gx[1].z = F1.x;

            Gy[1].y = F0.y;
            Gy[1].z = F1.y;

            Gz[1].y = F0.z;
            Gz[1].z = F1.z;

            Gx[2].y = F3.x;
            Gx[2].z = F2.x;

            Gy[2].y = F3.y;
            Gy[2].z = F2.y;

            Gz[2].y = F3.z;
            Gz[2].z = F2.z;

            float x = ifc::getMultplicationValue(Bu, Gx, Bv);
            float y = ifc::getMultplicationValue(Bu, Gy, Bv);
            float z = ifc::getMultplicationValue(Bu, Gz, Bv);

            vec4 point(x, y, z, 1);
            point = VP * point;

            point.x /= point.w;
            point.y /= point.w;

            points.push_back(point);
            vector<int> edge{index, index+1};
            edges.push_back(edge);
            index++;
        }
        u = u_max;
        vec4 Bu = cubicBernsteinVector(u);
        vec4 Bv = cubicBernsteinVector(v);

        vec3 F0 = solveF0(u, v);
        vec3 F1 = solveF1(u, v);
        vec3 F2 = solveF2(u, v);
        vec3 F3 = solveF3(u, v);

        Gx[1].y = F0.x;
        Gx[1].z = F1.x;

        Gy[1].y = F0.y;
        Gy[1].z = F1.y;

        Gz[1].y = F0.z;
        Gz[1].z = F1.z;

        Gx[2].y = F3.x;
        Gx[2].z = F2.x;

        Gy[2].y = F3.y;
        Gy[2].z = F2.y;

        Gz[2].y = F3.z;
        Gz[2].z = F2.z;

        float x = ifc::getMultplicationValue(Bu, Gx, Bv);
        float y = ifc::getMultplicationValue(Bu, Gy, Bv);
        float z = ifc::getMultplicationValue(Bu, Gz, Bv);

        vec4 point(x, y, z, 1);
        point = VP * point;

        point.x /= point.w;
        point.y /= point.w;
        points.push_back(point);
        index++;
    }

    setSurfaceColor(color);
    glLineWidth(this->lineWidth);
    glBegin(GL_LINES);
    for(unsigned int i = 1; i < edges.size(); i++){
        int index1 = edges[i][0];
        int index2 = edges[i][1];
        vec4& p1 = points[index1];
        vec4& p2 = points[index2];
        if(p1.w < 0 || p2.w < 0) continue;
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
    }
    glEnd();
}

void SurfaceFilling::renderDebug(const glm::mat4 &VP) {
    ObjectFactory &objectFactory = ObjectFactory::getInstance();
    commonPoints.point12->setColor(debugColors.CommonPointColor);
    commonPoints.point13->setColor(debugColors.CommonPointColor);
    commonPoints.point23->setColor(debugColors.CommonPointColor);

    for (unsigned int j = 0; j < 3; j++) {
        std::cout << "Surface:     "
        << fillingData[j].surface->getName() << std::endl;

        std::cout << "Var Param:   "
        << borderCurveParamToString(fillingData[j].varBorderCurveParam)
        << std::endl;
        std::cout << "Const Param: "
        << borderCurveParamToString(fillingData[j].constBorderCurveParam)
        << std::endl;
        std::cout << "Side:        "
        << borderCurveSideToString(fillingData[j].borderCurveSide)
        << std::endl;
        std::cout << " ------------ " << std::endl << std::endl;

        for (unsigned int i = 0; i < fillingData[j].borderPoints.size(); i++) {
            fillingData[j].borderPoints[i]->
                    setColor(debugColors.BorderControlPointsColor);
        }

        // P0 Middle Point
        ifc::Point *P0 = objectFactory.createPoint(to_string(j));
        P0->moveTo(fillingData[j].P0_BorderCurveMidPoint);
        P0->setColor(debugColors.BorderCurveMidPointColor);

        // P1: Tangent
        vec3 midPos = P0->getPosition();
        vec3 v2 = fillingData[j].P1_Tanget;
        vec4 v1 = vec4(midPos.x, midPos.y, midPos.z, 1.0f);
        vec4 v2_4 = vec4(v2.x, v2.y, v2.z, 1.0f);
        Line *tangetLine = objectFactory.createLine("line", v1, v2_4);
        tangetLine->setColor(debugColors.BorderCurveMidTangetColor);

        ifc::Point *P1 = objectFactory.createPoint(to_string(j));
        P1->moveTo(fillingData[j].P1_Tanget);
        P1->setColor(debugColors.P2Color);

        // P2
        ifc::Point *P2 = objectFactory.createPoint(to_string(j));
        P2->moveTo(fillingData[j].P2);
        P2->setColor(debugColors.P2Color);

        // P3
        ifc::Point *P3 = objectFactory.createPoint(to_string(j));
        P3->moveTo(fillingData[j].P3_Center);
        P3->setColor(debugColors.P3Color);

        // a0, a3, b3
        vec3 a0 = fillingData[j].a0PointLeft;
        vec3 a3 = fillingData[j].a3PointLeft;
        vec3 b3 = fillingData[j].b3PointLeft;

        vec4 vv2 = vec4(a0.x, a0.y, a0.z, 1.0f);
        Line *a0Line = objectFactory.createLine("line", v1, vv2);
        a0Line->setColor(debugColors.a0TangentColor);

        vec3 center = fillingData[j].P3_Center;
        vec4 center4 = vec4(center.x, center.y, center.z, 1.0f);
        vec4 a3_4 = vec4(a3.x, a3.y, a3.z, 1.0f);
        vec4 b3_4 = vec4(b3.x, b3.y, b3.z, 1.0f);

        Line *a3Line = objectFactory.createLine("line", center4, a3_4);
        a3Line->setColor(debugColors.a3TangentColor);
        Line *b3Line = objectFactory.createLine("line", center4, b3_4);
        b3Line->setColor(debugColors.b3TangentColor);

        // Bezier
        vector<ifc::Point *> bezierPoints{P0, P1, P2, P3};
        BezierSplineC0 *bezierSpline = objectFactory.createBezier("bezier",
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

        Line *g0Line = objectFactory.createLine("line", C0, g04);
        Line *g1Line = objectFactory.createLine("line", C05, g14);
        Line *g2Line = objectFactory.createLine("line", C1, g24);
        g0Line->setColor(debugColors.GGieldColor);
        g1Line->setColor(debugColors.GGieldColor);
        g2Line->setColor(debugColors.GGieldColor);

        // Di
        vec3 d0 = fillingData[j].DLeft[0];
        vec3 d1 = fillingData[j].DLeft[1];
        vec3 d2 = fillingData[j].DLeft[2];
        vec3 d3 = fillingData[j].DLeft[3];

        Line *d0Line = objectFactory.createLine("line", P0->getPosition(), d0);
        Line *d1Line = objectFactory.createLine("line", P1->getPosition(), d1);
        Line *d2Line = objectFactory.createLine("line", P2->getPosition(), d2);
        Line *d3Line = objectFactory.createLine("line", P3->getPosition(), d3);
        d0Line->setColor(debugColors.CenterBezierColor);
        d1Line->setColor(debugColors.CenterBezierColor);
        d2Line->setColor(debugColors.CenterBezierColor);
        d3Line->setColor(debugColors.CenterBezierColor);

        d0Line->setLineWidth(4.0f);
        d1Line->setLineWidth(4.0f);
        d2Line->setLineWidth(4.0f);
        d3Line->setLineWidth(4.0f);

        // Top Di
        vec3 d1Top = fillingData[j].DTop[1];
        vec3 d2Top = fillingData[j].DTop[2];

        Line *d1LineTop = objectFactory.createLine("line",
                                                   fillingData[j].left->P1_Tanget,
                                                   d1Top);
        Line *d2LineTop = objectFactory.createLine("line",
                                                   fillingData[j].left->P2,
                                                   d2Top);
        d1LineTop->setColor(debugColors.CenterBezierColor);
        d2LineTop->setColor(debugColors.CenterBezierColor);
        d1LineTop->setLineWidth(4.0f);
        d2LineTop->setLineWidth(4.0f);

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

        Line *g0TopLine = objectFactory.createLine("line", C0Top, g0Top);
        Line *g1TopLine = objectFactory.createLine("line", C05Top, g1Top);
        Line *g2TopLine = objectFactory.createLine("line", C1Top, g2Top);
        g0TopLine->setColor(debugColors.GGieldColor);
        g1TopLine->setColor(debugColors.GGieldColor);
        g2TopLine->setColor(debugColors.GGieldColor);

        // Half Bezier border Points
        ifc::Point *P00 = objectFactory.createPoint(to_string(j));
        ifc::Point *P01 = objectFactory.createPoint(to_string(j));
        ifc::Point *P02 = objectFactory.createPoint(to_string(j));
        ifc::Point *P03 = objectFactory.createPoint(to_string(j));

        P00->moveTo(fillingData[j].halfBezierPointsPointsBase[0]);
        P00->setColor(debugColors.HalfBezierControlPointColor);
        P01->moveTo(fillingData[j].halfBezierPointsPointsBase[1]);
        P01->setColor(debugColors.HalfBezierControlPointColor);
        P02->moveTo(fillingData[j].halfBezierPointsPointsBase[2]);
        P02->setColor(debugColors.HalfBezierControlPointColor);
        P03->moveTo(fillingData[j].halfBezierPointsPointsBase[3]);
        P03->setColor(debugColors.HalfBezierControlPointColor);

        // Inside Tangent Base
        Line *halfBezierLine1
                = objectFactory.createLine("line", P01->getPosition(),
                                           fillingData[j].halfBezierPoint1Base);
          /*
        Line *halfBezierLine1
                = objectFactory.createLine("line",
                                           fillingData[j].halfBezierPoint1BaseOrigin,
                                           fillingData[j].halfBezierPoint1Base);
        */
        halfBezierLine1->setLineWidth(4.0f);
        halfBezierLine1->setColor(debugColors.CenterBezierColor);

        Line *halfBezierLine2
                = objectFactory.createLine("line", P02->getPosition(),
                                           fillingData[j].halfBezierPoint2Base);

        /*
        Line *halfBezierLine2
                = objectFactory.createLine("line",
                                           fillingData[j]
                                                   .halfBezierPoint2BaseOrigin,
                                           fillingData[j].halfBezierPoint2Base);
                                           */
        halfBezierLine2->setLineWidth(4.0f);
        halfBezierLine2->setColor(debugColors.CenterBezierColor);


        ifc::Point *P00Top = objectFactory.createPoint(to_string(j));
        ifc::Point *P01Top = objectFactory.createPoint(to_string(j));
        ifc::Point *P02Top = objectFactory.createPoint(to_string(j));
        ifc::Point *P03Top = objectFactory.createPoint(to_string(j));

        P00Top->moveTo(fillingData[j].halfBezierPointsPointsTop[0]);
        P00Top->setColor(debugColors.HalfBezierControlPointColor);
        P01Top->moveTo(fillingData[j].halfBezierPointsPointsTop[1]);
        P01Top->setColor(debugColors.HalfBezierControlPointColor);
        P02Top->moveTo(fillingData[j].halfBezierPointsPointsTop[2]);
        P02Top->setColor(debugColors.HalfBezierControlPointColor);
        P03Top->moveTo(fillingData[j].halfBezierPointsPointsTop[3]);
        P03Top->setColor(debugColors.HalfBezierControlPointColor);

        Line *halfBezierTopLine1
                = objectFactory.createLine("line", P01Top->getPosition(),
                                           fillingData[j].halfBezierPoint1Top);

        /*
        Line *halfBezierTopLine1
                = objectFactory.createLine("line",
                                           fillingData[j]
                                                   .halfBezierPoint1TopOrigin,
                                           fillingData[j].halfBezierPoint1Top);
                                           */
        halfBezierTopLine1->setLineWidth(4.0f);
        halfBezierTopLine1->setColor(debugColors.CenterBezierColor);

        Line *halfBezierTopLine2
                = objectFactory.createLine("line", P02Top->getPosition(),
                                           fillingData[j].halfBezierPoint2Top);

        /*
        Line *halfBezierTopLine2
                = objectFactory.createLine("line",
                                           fillingData[j]
                                                   .halfBezierPoint2TopOrigin,
                                           fillingData[j].halfBezierPoint2Top);
        */
        halfBezierTopLine2->setLineWidth(4.0f);
        halfBezierTopLine2->setColor(debugColors.CenterBezierColor);


        //if (j == 0) {
        if (true) {
            P00->update();
            P00->render(VP);
            P01->update();
            P01->render(VP);
            P02->update();
            P02->render(VP);
            P03->update();
            P03->render(VP);

            a0Line->render(VP);
            a3Line->render(VP);
            b3Line->render(VP);

            g0Line->render(VP);
            g1Line->render(VP);
            g2Line->render(VP);

            g0TopLine->render(VP);
            g1TopLine->render(VP);
            g2TopLine->render(VP);

            d1Line->render(VP);
            (d2Line)->render(VP);
            (d1LineTop)->render(VP);
            (d2LineTop)->render(VP);

            (halfBezierLine1)->render(VP);
            (halfBezierLine2)->render(VP);
            (halfBezierTopLine1)->render(VP);
            (halfBezierTopLine2)->render(VP);
        }
        P0->update();
        P0->render(VP);
        P1->update();
        P1->render(VP);
        P2->update();
        P2->render(VP);
        P3->update();
        P3->render(VP);
        tangetLine->render(VP);

        (bezierSpline)->render(VP);

        delete P00;
        delete P01;
        delete P02;
        delete P03;

        delete a0Line;
        delete a3Line;
        delete b3Line;

        delete g0Line;
        delete g1Line;
        delete g2Line;

        delete g0TopLine;
        delete g1TopLine;
        delete g2TopLine;

        delete d1Line;
        delete d2Line;
        delete d1LineTop;
        delete d2LineTop;

        delete halfBezierLine1;
        delete halfBezierLine2;
        delete halfBezierTopLine1;
        delete halfBezierTopLine2;

        delete P0;
        delete P1;
        delete P2;
        delete P3;
        delete tangetLine;

        delete bezierSpline;
    }
}

//-----------------------//
//  PROTECTED
//-----------------------//

void SurfaceFilling::initVertices(){

}

void SurfaceFilling::initEdges(){

}

//-----------------------//
//  PUBLIC
//-----------------------//

bool SurfaceFilling::isRenderDebug(){
    return doRenderDebug;
}

void SurfaceFilling::setRenderDebug(bool v){
    doRenderDebug = v;
}

void SurfaceFilling::setRenderMode(RenderMode render_mode){
    renderMode = render_mode;
}

void SurfaceFilling::render(const glm::mat4 &VP){
    render(VP, this->color);
}

void SurfaceFilling::render(const glm::mat4 &VP, const Color &color) {
    findCommonPoints();
    if(fillingStatus != FillingStatus::COMPUTING) return;

    findBorderCurves();

    computeBorderCurveMidPoints();
    computeBorderCurveTangents();
    computeCenterPoints();
    computeGFieldVectors();
    computeHalfBorderCurvePoints();

    renderPatches(VP);
    if(doRenderDebug) renderDebug(VP);
}

void SurfaceFilling::update() {

}

FillingStatus SurfaceFilling::getStatus(){
    return this->fillingStatus;
}


