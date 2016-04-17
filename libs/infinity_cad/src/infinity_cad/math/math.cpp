//
// Created by jakub on 4/16/16.
//

#include "infinity_cad/math/math.h"

using namespace glm;

float ifc::dot(const vec3& v1, const vec3& v2){
    float dotValue = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

    return dotValue;
}

float ifc::euclideanDistance(const vec3& v1, const vec3& v2){
    float distance = 0;

    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    distance = dx*dx + dy*dy + dz*dz;

    distance = sqrt(distance);

    return distance;
}

float ifc::angleToRadians(float angle) {
    float radians = angle * PI_OVER_180;
    return radians;
}