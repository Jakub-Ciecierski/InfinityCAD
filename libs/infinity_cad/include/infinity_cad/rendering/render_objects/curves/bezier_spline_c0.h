//
// Created by jakub on 4/5/16.
//

#ifndef IC_BEZIER_CURVE_H
#define IC_BEZIER_CURVE_H

#include <infinity_cad/geometry/curves/bezier_cubic_curve.h>
#include "infinity_cad/rendering/render_objects/curves/spline.h"
#include <infinity_cad/rendering/color/color.h>
#include <infinity_cad/rendering/scene/scene_id.h>
#include <infinity_cad/rendering/render_objects/primitivies/point.h>

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Top Point has index 0
 * The last index is Bottom.
 *
 * Piecewise Bezier Curve, up to cubic degree.
 */
class BezierSplineC0 : public Spline{
private:
    std::vector<BezierCubicCurve> bezierCurves;
protected:

    virtual void buildCurve() override;

    virtual void draw(const glm::mat4 &VP, const Color& color) override;
    void drawCurves(const glm::mat4 &VP, const Color& color);
    void drawBezierPolygon(const glm::mat4 &VP, int SEGMENTS = 50);

    virtual void initVertices();

    virtual void initEdges();

public:

    BezierSplineC0(SceneID id, std::string name);
    BezierSplineC0(SceneID id, std::string name,
                   std::vector<ifc::Point*>& points);

    ~BezierSplineC0();

    glm::vec3 compute(float t);
    glm::vec4 computeVec4(float t);
};


#endif //IC_BEZIER_CURVE_H
