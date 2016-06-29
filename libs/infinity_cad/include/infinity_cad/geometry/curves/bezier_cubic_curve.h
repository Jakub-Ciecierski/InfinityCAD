//
// Created by jakub on 4/6/16.
//

#ifndef IC_BEZIERCUBICCURVE_H
#define IC_BEZIERCUBICCURVE_H

#include <infinity_cad/rendering/render_objects/primitivies/point.h>

class BezierCubicCurve {
private:
    ifc::Point* p0;
    ifc::Point* p1;
    ifc::Point* p2;
    ifc::Point* p3;

    std::vector<ifc::Point**> points;

public:
    static const int MAX_DEGREE_CUBIC = 3;
    static const int MAX_POINT_COUNT_CUBIC = 4;

    BezierCubicCurve();
    BezierCubicCurve(ifc::Point* P0, ifc::Point* P1,
                     ifc::Point* P2, ifc::Point* P3);

    glm::vec4 compute(float t);
    void addPoint(ifc::Point* p);

    ifc::Point* getPoint(int i);

    float getAverageDistanceBetweenPoints();
    float getMaximumDistanceBetweenPoints();

    int degree();

    bool isEmpty();
    bool isFull();
};


#endif //IC_BEZIERCUBICCURVE_H
