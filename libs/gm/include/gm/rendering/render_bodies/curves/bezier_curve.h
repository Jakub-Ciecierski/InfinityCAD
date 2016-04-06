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

    Color polygonColor;

    bool doDrawBezierPolygon;

    void buildBezierCurves_C0();

    void draw(const glm::mat4 &VP);
    void drawCurves(const glm::mat4 &VP);
    void drawBezierPolygon(const glm::mat4 &VP, int SEGMENTS = 50);


protected:
    virtual void initVertices();

    virtual void initEdges();

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

    void setDrawBezierPolygon(bool value);
    bool isDrawBezierPolygon();

    virtual float intersect(const RayCast &ray);
    virtual glm::vec3 getClosestPoint(const glm::vec3 point);

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;
};


#endif //IC_BEZIER_CURVE_H
