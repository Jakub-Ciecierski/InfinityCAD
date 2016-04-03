//
// Created by jakub on 1/9/16.
//

#include <stdexcept>
#include <math/random.h>
#include "math/hyper_sphere.h"

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

HyperSphere::HyperSphere(Point<double> center, double radius):
                center(center), radius(radius){
    dimensions = center.size();
    radiusSquared = this->radius * this->radius;

    this->hyperCubeMinMaxOfEachDimension
            = calculateHyperCubeMinMaxOfEachDimension();
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

bool HyperSphere::isInSphere(const Point<double>& point) const{
    int size = point.size();

    if(size != center.size()){
        throw std::invalid_argument("isInSphere dimension missmatch");
    }

    double differenceSum = 0;

    for(int i = 0; i < size; i++){
        double difference = point[i] - center[i];
        difference *= difference;
        differenceSum += difference;
    }

    return differenceSum <= radiusSquared;
}

Point<double>* HyperSphere::randomPointWithinDescribedHyperCube() const{

    Point<double>* pointWithinHyperCube
            = new Point<double>(dimensions);

    for(int i = 0; i < dimensions; i++){
        std::vector<double> minMaxVector =
                hyperCubeMinMaxOfEachDimension[i];
        double randomNumberWithinDimensionInterval =
                rnd::generateRandomNumber( minMaxVector[MIN_INDEX],
                                             minMaxVector[MAX_INDEX]);

        (*pointWithinHyperCube)[i] = randomNumberWithinDimensionInterval;
    }

    return pointWithinHyperCube;
}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

std::vector<std::vector<double>>
                    HyperSphere::calculateHyperCubeMinMaxOfEachDimension(){
    std::vector<std::vector<double>>
            minMaxOfEachDimension(dimensions);

    for(int i = 0; i < dimensions; i++){
        double min = center[i] - radius;
        double max = center[i] + radius;
        std::vector<double> minMax{min, max};
        minMaxOfEachDimension[i] = minMax;
    }

    return minMaxOfEachDimension;
}
