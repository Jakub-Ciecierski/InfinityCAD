//
// Created by jakub on 4/13/16.
//

#ifndef IC_BEZIER_SPLINE_H
#define IC_BEZIER_SPLINE_H

#include <gm/rendering/render_body.h>
#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/curves/bezier_cubic_curve.h>
#include <gm/rendering/render_bodies/curves/spline.h>

/*
 * Piecewise cubic curve. Bezier C2
 */
class BezierSpline : public Spline{
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

    BezierSpline(SceneID id, std::string name);
    BezierSpline(SceneID id, std::string name,
                std::vector<ic::Point*>& points);

    ~BezierSpline();

    float getDT();

    std::vector<BezierCubicCurve>& getCurves();

    virtual void update() override;
    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;
};

#endif //IC_BEZIER_SPLINE_H
