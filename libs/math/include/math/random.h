//
// Created by jakub on 11/12/15.
//

#ifndef AC_RANDOM_H
#define AC_RANDOM_H

#include "point.h"
#include <vector>
#include <random>

/*
 * Contains functions for random generation.
 */

namespace rnd
{
    static bool seeded = false;

    /*
     * Must be called once
     */
    inline void seed() {
        if (!seeded) {
            srand(time(NULL));
            seeded = true;
        }
    }

    template<class T>
    inline T generateRandomNumber(T startRange, T endRange) {
        return startRange + (T) rand() / ((T) RAND_MAX / (T) (endRange - startRange));
    }


    /*
     * Creates a vector of points generated using uniform distribution
     *
     * @count - the number of points.
     * @dim - the dimension size of each point.
     * @min - minimum value.
     * @max - maximum value.
     */
    template <typename T>
    std::vector<const Point<T>*> uniformPoints(int count,
                                               int dim, T min, T max);
}

#endif //AC_RANDOM_H
