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

float ifc::radiansToDegree(float rad){
    float deg = rad / PI_OVER_180;
    return deg;
}

bool ifc::equal(const glm::vec3& v1, const glm::vec3& v2, float error){
    float dist = euclideanDistance(v1,v2);
    return (dist >= 0 && dist <= error);
}

void ifc::solveTridiagonalSystem(std::vector<float>& belowDiagonal,
                                 std::vector<float>& mainDiagonal,
                                 std::vector<float>& aboveDiagonal,
                                 std::vector<float>& d){
    int n = mainDiagonal.size();

    aboveDiagonal[0] /= mainDiagonal[0];
    d[0] /= mainDiagonal[0];

    for(int i = 1; i < n; i++){
        aboveDiagonal[i] /= mainDiagonal[i] -
                belowDiagonal[i] * aboveDiagonal[i - 1];
        d[i] = (d[i] - belowDiagonal[i] * d[i - 1]) /
                (mainDiagonal[i] - belowDiagonal[i] * aboveDiagonal[i - 1]);
    }
    for(int i = n-1; i-- > 0;){
        d[i] -= aboveDiagonal[i] * d[i + 1];
    }
}

float ifc::xPixelToGLCoord(int p, int width) {
    float vX = 2.0 / (float) width;
    float glX = vX * (float)p - 1.0;

    return glX;
}

float ifc::yPixelToGLCoord(int p, int height) {
    float vY = 2.0 / (float) height;
    float glY = (vY * (float)p - 1.0) * -1;

    return glY;
}

int ifc::xGLToPixelCoord(float x, int width) {
    float vX = 2.0 / (float) width;
    int pX = (x + 1.0f) / vX;

    return pX;
}

int ifc::yGLToPixelCoord(float y, int height) {
    float vY = 2.0 / (float) height;
    int pY = (y + 1.0f) / vY;



    return height - pY;
}
