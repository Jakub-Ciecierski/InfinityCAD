//
// Created by jakub on 3/5/16.
//

#include <gm/angle_to_radians.h>
#include <math.h>

const float PI_OVER_180 = M_PI/180;

float angleToRadians(float angle) {
    float radians = angle * PI_OVER_180;
    return radians;
}