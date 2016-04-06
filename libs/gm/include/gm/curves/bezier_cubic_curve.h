//
// Created by jakub on 4/6/16.
//

#ifndef IC_BEZIERCUBICCURVE_H
#define IC_BEZIERCUBICCURVE_H


#include <gm/rendering/render_bodies/primitivies/point.h>

class BezierCubicCurve {
private:
    ic::Point* p0;
    ic::Point* p1;
    ic::Point* p2;
    ic::Point* p3;

public:
    static const int MAX_DEGREE_CUBIC = 3;
    static const int MAX_POINT_COUNT_CUBIC = 4;

    BezierCubicCurve();

    glm::vec4 compute(float t);

    void addPoint(ic::Point* p);

    int degree();

    bool isEmpty();
    bool isFull();
};


#endif //IC_BEZIERCUBICCURVE_H
