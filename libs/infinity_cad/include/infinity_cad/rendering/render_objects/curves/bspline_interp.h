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
 *
 * Sources:
 * http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/INT-APP/SURF-INT-global.html
 */
class BSplineInterp : public Spline{
private:
    float interpolatingPolygonLength;

    int MAX_PIXEL_COUNT = 10000;
    glm::vec4* curvePoints;

    int DEGREE = 3;
    int DIMENSION = 3;
    int MIN_POINTS = 4;

    std::vector<float> parameters;
    std::vector<float> knotVector;

    std::vector<glm::vec3> controlPoints;

    void computeChordParameters();
    void computeKnotVector(int degree);
    void computeControlPoints(int degree);
    void computeControlPointsDeg3(int degree);
    void computeControlPointsDeg12(int degree);

    void drawSplineGPU(const glm::mat4 &VP, const Color& color,
                       int pixelCount, float t, float dt, int degree);
    void drawSplineCPU(const glm::mat4 &VP, const Color& color,
                       int degree, float t, float t_max, float dt);

    void updateCurve();

protected:

    virtual void draw(const glm::mat4 &VP, const Color& color) override;
    virtual void buildCurve() override;

public:

    BSplineInterp(SceneID sceneID, std::string name);

    ~BSplineInterp();

};


#endif //IC_BSPLINEINTERP_H
