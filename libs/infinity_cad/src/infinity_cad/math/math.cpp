//
// Created by jakub on 4/16/16.
//

#include <iostream>
#include "infinity_cad/math/math.h"

using namespace glm;
using namespace std;

float ifc::dot(const vec3& v1, const vec3& v2){
    float dotValue = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

    return dotValue;
}

float ifc::euclideanDistance(const vec4& v1, const vec4& v2){
    float distance = 0;

    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    float dw = v1.w - v2.w;
    distance = dx*dx + dy*dy + dz*dz + dw*dw;

    distance = sqrt(distance);

    return distance;
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

float ifc::euclideanDistance(const vec2& v1, const vec2& v2){
    float distance = 0;

    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    distance = dx*dx + dy*dy;

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

float ifc::getMultplicationValue(const vec4& v1, const mat4& m, const vec4& v2){
    vec4 res = v1 * m * v2;
    float val = res.x + res.y + res.z + res.w;

    return val;
}

void ifc::printVec2(const glm::vec2& vec){
    std::cout << vec.x << ", " << vec.y << std::endl;
}

void ifc::printVec3(const glm::vec3& vec){
    std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
}

void ifc::printVec4(const glm::vec4& vec){
    std::cout
    << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << std::endl;
}

bool ifc::isNan(const glm::vec4& v){
    return (isnan(v.x) || isnan(v.y) || isnan(v.z) ||isnan(v.w));
}

float ifc::derivative(function<float(float, float, float, float)> f,
                      float x, float y, float z, float w,
                      DerivativeTypes derivativeType) {
    //float h = 0.00001;
    float h = 0.00000001;
    float h2 = h * 2;

    float value = 0.0f;

    if(derivativeType == DerivativeTypes::DX){
        float v1 = f(x - h2, y, z, w);
        float v2 = 8 * f(x - h, y, z, w);
        float v3 = 8 * f(x +h, y, z, w);
        float v4 = f(x + h2, y, z, w);
        value = (v1 - v2 + v3 - v4) / (h2*6);
    }else if(derivativeType == DerivativeTypes::DY){
        value =
                (f(x, y - h2, z, w)
                 - 8 * f(x, y - h, z, w)
                 + 8 * f(x, y + h, z, w)
                 - f(x, y + h2, z, w)) / (h2 * 6);
    }else if(derivativeType == DerivativeTypes::DZ){
        value =
                (f(x, y, z - h2, w)
                 - 8 * f(x, y, z - h, w)
                 + 8 * f(x, y, z + h, w)
                 - f(x, y, z + h2, w)) / (h2 * 6);
    }else if(derivativeType == DerivativeTypes::DW){
        value =
                (f(x, y, z, w - h2)
                 - 8 * f(x, y, z, w - h)
                 + 8 * f(x, y, z, w + h)
                 - f(x, y, z, w + h2)) / (h2 * 6);
    }
    if(isnan(value)) value = 0.0f;
    return value;
}