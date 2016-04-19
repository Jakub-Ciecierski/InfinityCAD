//
// Created by jakub on 4/18/16.
//

#ifndef IC_BSPLINEINTERP_H
#define IC_BSPLINEINTERP_H


#include <infinity_cad/rendering/render_objects/curves/spline.h>

/*
 * BSpline Interpolating sequence of points.
 * In this case the input points are not control points,
 * but the interpolating points.
 * Control points are computed by System of Linear equations
 * using Thomas Algorithm.
 */
class BSplineInterp : public Spline{
private:
    int DEGREE = 3;

    std::vector<float> parameters;
    std::vector<float> knotVector;
    std::vector<glm::vec3> controlPoints;

    void computeChordParameters();
    void computeKnotVector();
    void computeControlPoints();

protected:

    virtual void draw(const glm::mat4 &VP, const Color& color) override;
    virtual void buildCurve() override;

public:

    BSplineInterp(SceneID sceneID, std::string name);

    ~BSplineInterp();

};


#endif //IC_BSPLINEINTERP_H
