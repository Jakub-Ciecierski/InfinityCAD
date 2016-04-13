//
// Created by jakub on 4/10/16.
//

#ifndef IC_BSPLINE_H
#define IC_BSPLINE_H

#include <gm/rendering/render_body.h>
#include <gm/rendering/render_bodies/curves/spline.h>

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

    int computeDegree();

protected:
    virtual void initVertices();

    virtual void initEdges();

    virtual void draw(const glm::mat4 &VP, const Color& color) override;

    virtual void buildCurve() override;
public:

    BSpline(SceneID id, std::string name);

    ~BSpline();

    virtual float intersect(const RayCast &ray);
    virtual glm::vec3 getClosestPoint(const glm::vec3 point);

    int getDegree();
    int getControlPointCount();

    int getKnotCount();

    float getKnotMin(int degree);
    float getKnotMax();
    const std::vector<float>& getKnotVector();
};


#endif //IC_BSPLINE_H
