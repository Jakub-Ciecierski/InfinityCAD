//
// Created by jakub on 6/25/16.
//

#ifndef IC_SURFACE_FILLING_H
#define IC_SURFACE_FILLING_H

#include <infinity_cad/rendering/render_objects/surfaces/surface.h>
#include <infinity_cad/rendering/scene/scene.h>
#include <infinity_cad/rendering/render_objects/primitivies/line.h>
#include <infinity_cad/rendering/render_objects/curves/bezier_spline_c0.h>

/**
 * http://research.microsoft.com/en-us/um/people/cloop/SGA09.pdf
 */

enum BorderCurveParam {
    U, V, U0, U1, V0, V1
};

/**
 * Computed based on combination of constant and variable of BorderCurveParam.
 * */
enum BorderCurveSide {
    TOP, BOTTOM, LEFT, RIGHT
};

enum RenderMode{
    RENDER_ALL,
    RENDER_FIRST_PATCH,
    RENDER_SECOND_PATCH,
    RENDER_THIRD_PATCH
};

enum FillingStatus{
    COMPUTING, UNKNOWN_ERROR, COMPLETE, NO_HOLE
};

/**
 * On which side
 */
enum HalfBezierWhichSide{
    NO_SIDE, U_FIRST, U_SECOND, V_FIRST, V_SECOND
};

enum CommonPointPosition {
    TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
};

/**
 * Used to compute Half Bezier of the border curve
 */
struct HalfBorderCurveData {
    float start;
    float end;
    float p;
    float k;
};

struct CommonPoints{
    ifc::Point* point12;
    ifc::Point* point13;
    ifc::Point* point23;
};

struct CommonPointResult{
    ifc::Point* point;
    CommonPointPosition position1;
    CommonPointPosition position2;
};

struct FillingData{
    Surface* surface;

    FillingData* left;
    FillingData* right;

    // Commons Points shared between neighbour border.
    ifc::Point* commonPoint1;
    ifc::Point* commonPoint2;

    // Border points make up border curve.
    std::vector<ifc::Point*> borderPoints;

    // Is the curve drawn by u or v parameter
    BorderCurveParam varBorderCurveParam;
    BorderCurveParam constBorderCurveParam;

    BorderCurveSide borderCurveSide;
    // Which side of the border curve to compute half bezier curve
    HalfBezierWhichSide half_bezier_which_side;
    HalfBezierWhichSide half_bezier_which_side_top;

    glm::vec3 borderCurveTanget;

    glm::vec3 a0TangetLeft;
    glm::vec3 b0TangetLeft;
    glm::vec3 a3TangetLeft;
    glm::vec3 b3TangetLeft;

    glm::vec3 a0PointLeft;
    glm::vec3 b0PointLeft;
    glm::vec3 a3PointLeft;
    glm::vec3 b3PointLeft;

    glm::vec3 a0PointTop;
    glm::vec3 b0PointTop;
    glm::vec3 a3PointTop;
    glm::vec3 b3PointTop;

    glm::vec3 g0TangentLeft;
    glm::vec3 g1TangentLeft;
    glm::vec3 g2TangentLeft;

    glm::vec3 g0PointLeft;
    glm::vec3 g1PointLeft;
    glm::vec3 g2PointLeft;

    glm::vec3 g0PointTop;
    glm::vec3 g1PointTop;
    glm::vec3 g2PointTop;

    // P0
    glm::vec3 P0_BorderCurveMidPoint;
    // P1 = P0 + borderCurveTanget
    glm::vec3 P1_Tanget;
    // P2
    glm::vec3 P2;
    // P3
    glm::vec3 P3_Center;

    // Di 2 control points. "Inside"
    std::vector<glm::vec3> DLeft;
    std::vector<glm::vec3> DTop;

    std::vector<glm::vec3> halfBezierPointsPointsBase;
    glm::vec3 halfBezierTanget1Base;
    glm::vec3 halfBezierTanget2Base;

    // Inside
    glm::vec3 halfBezierPoint1Base;
    glm::vec3 halfBezierPoint2Base;
    glm::vec3 halfBezierPoint1BaseOrigin;
    glm::vec3 halfBezierPoint2BaseOrigin;

    std::vector<glm::vec3> halfBezierPointsPointsTop;
    glm::vec3 halfBezierTanget1Top;
    glm::vec3 halfBezierTanget2Top;

    //Inside
    glm::vec3 halfBezierPoint1Top;
    glm::vec3 halfBezierPoint2Top;
    glm::vec3 halfBezierPoint1TopOrigin;
    glm::vec3 halfBezierPoint2TopOrigin;

};

struct DebugObjects{
    ifc::Point* P0;
    ifc::Point* P1;
    ifc::Point* P2;
    ifc::Point* P3;

    Line* tangetLine;


    Line* a0Line;
    Line* a3Line;
    Line* b3Line;
    BezierSplineC0* bezierSpline;

    Line* g0Line;
    Line* g1Line;
    Line* g2Line;

    Line* d0Line;
    Line* d1Line;
    Line* d2Line;
    Line* d3Line;

    Line* d1LineTop;
    Line* d2LineTop;

    Line* g0TopLine;
    Line* g1TopLine;
    Line* g2TopLine;

    ifc::Point* P00;
    ifc::Point* P01;
    ifc::Point* P02;
    ifc::Point* P03;

    Line* halfBezierLine1;
    Line* halfBezierLine2;

    ifc::Point* P00Top;
    ifc::Point* P01Top;
    ifc::Point* P02Top;
    ifc::Point* P03Top;

    Line* halfBezierTopLine1;
    Line* halfBezierTopLine2;


};

struct DebugColors{
    Color CommonPointColor;
    Color BorderControlPointsColor;

    Color BorderCurveMidPointColor;
    Color BorderCurveMidTangetColor;

    Color a0TangentColor;
    Color a3TangentColor;
    Color b3TangentColor;

    Color GGieldColor;

    Color P2Color;
    Color P3Color;

    Color CenterBezierColor;

    Color HalfBezierControlPointColor;
};

/*
 * Assumes that Surfaces are C0 single patches
 */
class SurfaceFilling : public RenderObject{
private:
    FillingData fillingData[3];

    DebugColors debugColors;
    RenderMode renderMode;
    CommonPoints commonPoints;
    FillingStatus fillingStatus;

    int counter;

    const HalfBorderCurveData FIRST_HALF_BORDER_CURVE_DATA{
            0.0f, 0.5f, 1.0f / 6.0f, 2.0f / 6.0f
    };
    const HalfBorderCurveData SECOND_HALF_BORDER_CURVE_DATA{
            0.5f, 1.0f, 4.0f / 6.0f, 5.0f / 6.0f
    };

    const HalfBorderCurveData U_FIRST_DATA {
            0.0f, 0.5f, 1.0f / 6.0f, 2.0f / 6.0f
    };
    const HalfBorderCurveData U_SECOND_DATA {
            0.5f, 1.0f, 4.0f / 6.0f, 5.0f / 6.0f
    };
    const HalfBorderCurveData V_FIRST_DATA {
            0.5f, 1.0f, 4.0f / 6.0f, 5.0f / 6.0f
    };
    const HalfBorderCurveData V_SECOND_DATA {
            0.0f, 0.5f, 1.0f / 6.0f, 2.0f / 6.0f
    };

    // ----------------

    /*
     * Finds Common control points among the surfaces
     */
    void findCommonPoints();
    CommonPointResult findCommonPoint(Surface* surface1, Surface* surface2);
    void updateStatusCommonPoints(CommonPoints &commonPoints);

    /*
     * Finds the border curve of each surface
     */
    void findBorderCurves();
    void findBorderCurve(FillingData &fillingData);
    void computeBorderCurveSideType(FillingData &fillingData);

    /*
     * Computes P0
     */
    void computeBorderCurveMidPoints();
    void computerBorderCurveMidPoint(FillingData& fillingData);

    /*
     * Computes P1
     */
    void computeBorderCurveTangents();
    void computeBorderCurveTangent(FillingData& fillingData);

    /*
     * Computes P2, P3
     */
    void computeCenterPoints();
    glm::vec3 computeAuxiliaryPointQ(FillingData &fillingData);

    /*
     * Computes a0, b0, a3, b3, g0,g1,g2
     */
    void computeGFieldVectors();
    void computeGFieldVector(FillingData &fillingData);
    void computeGFieldVectorTop(FillingData& fillingDataBase);
    void computeD(FillingData &fillingData);
    void computeDTop(FillingData &fillingData);

    /*
     *
     */
    void computeHalfBorderCurvePoints();
    void computeHalfBorderCurvePoints(FillingData& fillingData);
    void computeHalfBorderCurvePointsTop(FillingData& fillingData);
    std::vector<glm::vec3> CalculateBezierControlPoints(glm::vec3 p0,
                                                        glm::vec3 p1,
                                                        glm::vec3 p2,
                                                        glm::vec3 p3,
                                                        float u, float v);

    glm::vec2 getUV(FillingData& fillingData);
    std::string borderCurveParamToString(BorderCurveParam param);
    std::string borderCurveSideToString(BorderCurveSide param);
    std::string pointSurfaceTypeToString(ifc::PointSurfaceType param);
    std::string pointIndexToString(PointIndex point_index);
    std::string commonPointPositionToString(CommonPointPosition position);
    std::string halfBezierWhichSideToString(HalfBezierWhichSide side);
    CommonPointPosition determinePointPosition(PointIndex index);

    /**
     * Determines the parameters to properly compute point on the
     * half bezier curve
     */
    HalfBezierWhichSide determineHalfBezierSide(
            BorderCurveSide border_curve_side,
            CommonPointPosition common_point_position);

    HalfBezierWhichSide determineHalfBezierOpositeSide(
            BorderCurveSide border_curve_side,
            CommonPointPosition common_point_position);

    const HalfBorderCurveData getHalfBezierCurveDataBasedOnSide(
            HalfBezierWhichSide side);

    void renderPatches(const glm::mat4 &VP);
    void renderPatch(const glm::mat4 &VP, FillingData& fillingData);

    void renderDebug(const glm::mat4 &VP);
protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

    bool doRenderDebug;
    float TANGET_MULTIPLIER;
public:

    SurfaceFilling(SceneID id, std::string name,
                   Surface* surface1, Surface* surface2, Surface* surface3);

    ~SurfaceFilling();

    bool isRenderDebug();
    void setRenderDebug(bool v);

    void setRenderMode(RenderMode render_mode);

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;

    virtual void update() override;

    FillingStatus getStatus();
};


#endif //IC_SURFACE_FILLING_H

