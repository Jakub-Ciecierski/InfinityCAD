//
// Created by jakub on 1/9/16.
//

#ifndef BACHELOR_THESIS_HYPER_SPHERE_H
#define BACHELOR_THESIS_HYPER_SPHERE_H

#include <math/point.h>

class HyperSphere {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    Point<double> center;
    int dimensions;

    double radius;
    double radiusSquared;

    // Constant indices used to access the min max vector
    const int MIN_INDEX = 0;
    const int MAX_INDEX = 1;

    // Min and Max values (interval) of each dimension of
    // described hypercube.
    std::vector<std::vector<double>> hyperCubeMinMaxOfEachDimension;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    /*
     * Calculates min and max of each dimension.
     */
    std::vector<std::vector<double>> calculateHyperCubeMinMaxOfEachDimension();

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    HyperSphere(Point<double> center, double radius);

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    bool isInSphere(const Point<double>& point) const;

    /*
     * Generates a random point within a described hypercube.
     *
     * Allocates memory.
     */
    Point<double>* randomPointWithinDescribedHyperCube() const;
};


#endif //BACHELOR_THESIS_HYPER_SPHERE_H
