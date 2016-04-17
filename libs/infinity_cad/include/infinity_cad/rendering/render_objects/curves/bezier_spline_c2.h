//
// Created by jakub on 4/13/16.
//

#ifndef IC_BEZIER_SPLINE_H
#define IC_BEZIER_SPLINE_H

#include <infinity_cad/rendering/render_objects/primitivies/point.h>
#include "infinity_cad/rendering/render_objects/curves/spline.h"
#include <infinity_cad/rendering/color/color.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <infinity_cad/geometry/curves/bezier_cubic_curve.h>


/*
 * Piecewise cubic curve. Bezier C2
 */
class BezierSplineC2 : public Spline{
private:
    std::vector<BezierCubicCurve> bezierCurves;

    float dt;

    void C2Continuity();
protected:

    virtual void buildCurve() override;

    virtual void draw(const glm::mat4 &VP, const Color& color) override;
    void drawCurves(const glm::mat4 &VP, const Color& color);
    void drawBezierPolygon(const glm::mat4 &VP, int SEGMENTS = 50);

    virtual void initVertices();

    virtual void initEdges();

public:

    BezierSplineC2(SceneID id, std::string name);
    BezierSplineC2(SceneID id, std::string name,
                   std::vector<ifc::Point*>& points);

    ~BezierSplineC2();

    float getDT();

    std::vector<BezierCubicCurve>& getCurves();

    virtual void update() override;
    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;
};

#endif //IC_BEZIER_SPLINE_H
