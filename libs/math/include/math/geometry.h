//
// Created by jakub on 11/11/15.
//

#ifndef AC_GEOMETRY_H
#define AC_GEOMETRY_H

#include "point.h"

/*
 * Contains useful geometry functions
 */
namespace math
{

    /*
    * Returns the euclidean distance between two points.
     *
     * @p1 - point one.
     * @p2 - point two.
    */
    template<typename T>
    double euclideanDistance(const Point<T> &p1, const Point<T> &p2);

    /*
     * Computes the mean point - the center of mass.
     *
     * @points - the vector of points to compute center of mass for.
     */
    template<typename T>
    Point<T> centerOfMass(const std::vector<Point<T>>* points);

    /*
     * Bounds destPoint such that the delta of each dimension
     * between srcPoint and destPoint is no bigger than maxDelta.
     */
    Point<double> boundPoint(const Point<double>& srcPoint,
                             const Point<double>& destPoint,
                             double maxDelta);
}

#endif //AC_GEOMETRY_H
