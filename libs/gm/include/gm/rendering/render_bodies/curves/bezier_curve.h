//
// Created by jakub on 4/5/16.
//

#ifndef IC_BEZIER_CURVE_H
#define IC_BEZIER_CURVE_H

#include <gm/rendering/render_body.h>
#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/curves/bezier_cubic_curve.h>

/**
 * Top Point has index 0
 * The last index is Bottom.
 *
 * Piecewise Bezier Curve, up to cubic degree.
 */
class BezierCurve : public RenderBody{
private:
    std::vector<ic::Point*> points;
    std::vector<BezierCubicCurve> bezierCurves;

    void printStatusDEBUG();
    void buildBezierCurves_C0();

protected:
    virtual void initVertices();

    virtual void initEdges();

    void drawCurves(const glm::mat4 &VP);

public:

    BezierCurve(SceneID id, std::string name);
    BezierCurve(SceneID id, std::string name,
                std::vector<ic::Point*>& points);

    ~BezierCurve();

    void addPoint(ic::Point* point);
    void removePoint(ic::Point* point);

    void swapPoints(int i, int j);

    void moveUp(ic::Point* point);
    void moveDown(ic::Point* point);

    int getPointIndex(ic::Point* point);
    const std::vector<ic::Point*>& getPoints();

    virtual float intersect(const RayCast &ray);
    virtual glm::vec3 getClosestPoint(const glm::vec3 point);

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;
};


#endif //IC_BEZIER_CURVE_H
