//
// Created by jakub on 1/19/16.
//

#ifndef BACHELOR_THESIS_MATH_H
#define BACHELOR_THESIS_MATH_H

#include <vector>
#include <algorithm>
#include <cmath>

namespace math{

    /*
     * Returns the maximum of all objects in the vector
     */
    double max(const std::vector<double>& objects){
        auto it = std::max_element(std::begin(objects), std::end(objects));
        return *it;
    }

}

#endif //BACHELOR_THESIS_MATH_H
