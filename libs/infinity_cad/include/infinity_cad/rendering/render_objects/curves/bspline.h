//
// Created by jakub on 4/10/16.
//

#ifndef IC_BSPLINE_H
#define IC_BSPLINE_H

#include "infinity_cad/rendering/render_objects/curves/spline.h"
#include "infinity_cad/rendering/render_objects/curves/bezier_spline_c2.h"
#include <infinity_cad/rendering/color/color.h>
#include <infinity_cad/rendering/scene/scene_id.h>
#include <infinity_cad/rendering/render_objects/primitivies/point.h>
#include <infinity_cad/rendering/visibility/ray_cast.h>
#include <infinity_cad/rendering/render_object.h>

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>


/**
 * The control points in BSpline are called de Boor points.
 *
 * Knot Vector is defined based on degree and number of control points.
 * Let  n = number of control points.
 *      d = degree
 *      m = n + d = number of knots
 */
class BSpline : public Spline{
private:
    std::vector<float> knotVector;
    int MAX_DEGREE = 3;

    BezierSplineC2 * bezierSpline;
    bool drawBezierBasis;

    int MAX_PIXEL_COUNT = 100000;
    glm::vec4* curvePoints;

    int computeDegree();
protected:
    virtual void initVertices();

    virtual void initEdges();


    virtual void draw(const glm::mat4 &VP, const Color& color) override;
    void drawBezierSpline(const glm::mat4 &VP, const Color& color);
    void drawSpline(const glm::mat4 &VP, const Color& color);

    void drawSplineCPU(const glm::mat4 &VP, const Color& color,
                       int degree, float t, float t_max, float dt);

    void drawSplineGPU(const glm::mat4 &VP, const Color& color,
                       int pixelCount, float t, float dt, int degree);

    void drawPolygon(const glm::mat4 &VP, int SEGMENTS = 50);

    virtual void buildCurve() override;
    void buildBezierSpline();
    std::vector<glm::vec4> calculateBezierPoints();
public:

    BSpline(SceneID id, std::string name);

    ~BSpline();

    virtual float intersect(const RayCast &ray);
    virtual glm::vec3 getClosestPoint(const glm::vec3 point);

    bool isDrawBezierBasis();
    void setDrawBezierBasis(bool value);

    int getDegree();
    int getControlPointCount();

    int getKnotCount();

    float getKnotMin(int degree);
    float getKnotMax();
    const std::vector<float>& getKnotVector();

    virtual const std::vector<RenderObject *> &getChildren() override;
    virtual void update() override;

    virtual void setDrawBezierPolygon(bool value) override;
};


#endif //IC_BSPLINE_H
