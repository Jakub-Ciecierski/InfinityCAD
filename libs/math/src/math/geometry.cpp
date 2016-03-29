//
// Created by jakub on 11/11/15.
//

#include "math/geometry.h"

#include <stdexcept>
#include <math.h>
#include <vector>

namespace math
{

    /*
     * Returns the distance between two points
     */
    template<typename T>
    double euclideanDistance(const Point<T> &p1, const Point<T> &p2){
        double dist;
        int size1, size2;

        size1 = p1.size();
        size2 = p2.size();

        if (size1 != size2) {
            throw std::invalid_argument("The dimensions must match");
        }

        dist = 0;
        for(int i = 0; i < size1; i++){
            T x = p1[i]-p2[i];

            dist += x*x;
        }

        dist = sqrt(dist);

        return dist;
    }

    /*
     * Computes the mean point - the center of mass.
     *
     * Return empty point if container is empty.
     */
    template<typename T>
    Point<T> centerOfMass(const std::vector<Point<T>>* points){
        int count = points->size();

        // Return null if container is empty
        if (count == 0)
            return Point<T>();

        // Check if dimensions match
        int size = (*points)[0].size();
        for(int i = 1; i < size; i++){
            if(size != (*points)[i].size())
                throw std::invalid_argument("Dimensions must match");
        }

        Point<T> mean(size);

        for(int i = 0;i < size; i++){
            mean[i] = 0;
        }

        for(int i = 0;i < count; i++){
            mean += (*points)[i];
        }

        mean /= count;

        return mean;
    }

    Point<double> boundPoint(const Point<double>& srcPoint,
                             const Point<double>& destPoint,
                             double maxDelta){
        Point<double> boundedPoint(srcPoint.size());

        for(int i = 0;i < destPoint.size(); i++){
            int sign;

            sign = 1;
            double delta = (srcPoint[i] - destPoint[i]);
            if (delta < 0)
                sign = -1;

            delta *= delta;
            delta = sqrt(delta);

            if(delta > maxDelta){
                boundedPoint[i] = srcPoint[i] - (maxDelta * sign);
            }
            else{
                boundedPoint[i] = destPoint[i];
            }
        }

        return boundedPoint;
    }

    //-----------------------------------------------------------//
    //  TEMPLATE DECLARATIONS
    //-----------------------------------------------------------//

    template double euclideanDistance(const Point<char> &, const Point<char> &);
    template double euclideanDistance(const Point<int> &, const Point<int> &);
    template double euclideanDistance(const Point<float> &,
                                      const Point<float> &);
    template double euclideanDistance(const Point<double> &,
                                      const Point<double> &);

    //-----------------------------------------------------------//

    template Point<char> centerOfMass(const std::vector<Point<char>>*);
    template Point<int> centerOfMass(const std::vector<Point<int>>*);
    template Point<float> centerOfMass(const std::vector<Point<float>>*);
    template Point<double> centerOfMass(const std::vector<Point<double>>*);

}

