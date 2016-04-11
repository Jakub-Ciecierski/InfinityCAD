//
// Created by jakub on 4/5/16.
//

#ifndef IC_BEZIER_CURVE_H
#define IC_BEZIER_CURVE_H

#include <gm/rendering/render_body.h>
#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/curves/bezier_cubic_curve.h>
#include <gm/rendering/render_bodies/curves/spline.h>

/**
 * Top Point has index 0
 * The last index is Bottom.
 *
 * Piecewise Bezier Curve, up to cubic degree.
 */
class BezierCurve : public Spline{
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

    BezierCurve(SceneID id, std::string name);
    BezierCurve(SceneID id, std::string name,
                std::vector<ic::Point*>& points);

    ~BezierCurve();

};


#endif //IC_BEZIER_CURVE_H
