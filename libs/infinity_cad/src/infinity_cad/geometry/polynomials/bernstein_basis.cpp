//
// Created by jakub on 4/6/16.
//

#include <infinity_cad/geometry/polynomials/bernstein_basis.h>

using namespace glm;
using namespace std;

vec4 linearBernstein(const vec3& p0, const vec3& p1,
                     float t){
    float B0 = (1-t);
    float B1 = t;

    float x = (p0.x * B0) + (p1.x * B1);
    float y = (p0.y * B0) + (p1.y * B1);
    float z = (p0.z * B0) + (p1.z * B1);

    return vec4(x,y,z,1);
}

vec4 quadraticBernstein(const vec3& p0, const vec3& p1,
                        const vec3& p2, float t){
    float t2 = t*t;

    float B0 = pow(1-t, 2);
    float B1 = (2*t - 2*t2);
    float B2 = t2;

    float x = (p0.x * B0) + (p1.x * B1) + (p2.x * B2);
    float y = (p0.y * B0) + (p1.y * B1) + (p2.y * B2);
    float z = (p0.z * B0) + (p1.z * B1) + (p2.z * B2);

    return vec4(x,y,z,1);
}

vec3 quadraticBernstein3(const vec3& p0, const vec3& p1,
                         const vec3& p2, float t){
    float t2 = t*t;

    float B0 = pow(1-t, 2);
    float B1 = (2*t - 2*t2);
    float B2 = t2;

    float x = (p0.x * B0) + (p1.x * B1) + (p2.x * B2);
    float y = (p0.y * B0) + (p1.y * B1) + (p2.y * B2);
    float z = (p0.z * B0) + (p1.z * B1) + (p2.z * B2);

    return vec3(x,y,z);
}

/*
vec4 cubicBernstein(const vec3& p0, const vec3& p1,
                    const vec3& p2, const vec3& p3,
                    float t){

    float t2 = t*t;
    float t3 = t*t*t;

    float B0 = 1 - 3*t + 3*t2 - t3;
    float B1 = 3*t - 6*t2 + t3;
    float B2 = 3*t2 - 3*t3;
    float B3 = t3;

    float x = (p0.x * B0) + (p1.x * B1) + (p2.x * B2) + (p3.x * B3);
    float y = (p0.y * B0) + (p1.y * B1) + (p2.y * B2) + (p3.y * B3);
    float z = (p0.z * B0) + (p1.z * B1) + (p2.z * B2) + (p3.z * B3);

    return vec4(x,y,z,1);
}
 */

vec4 cubicBernstein(const vec3& p0, const vec3& p1,
                    const vec3& p2, const vec3& p3,
                    float t){
/*
    float t2 = t*t;
    float t3 = t*t*t;

    float B0 = 1 - 3*t + 3*t2 - t3;
    float B1 = 3*t - 6*t2 + 3*t3;
    float B2 = 3*t2 - 3*t3;
    float B3 = t3;
*/
    float x = pow(1-t, 3) * p0.x +
              pow(1-t, 2) * 3*t * p1.x +
            (1-t) * 3 *t *t * p2.x +
            t*t*t*p3.x;
    float y = pow(1-t, 3) * p0.y +
              pow(1-t, 2) * 3*t * p1.y +
              (1-t) * 3 *t *t * p2.y +
              t*t*t*p3.y;
    float z = pow(1-t, 3) * p0.z +
              pow(1-t, 2) * 3*t * p1.z +
              (1-t) * 3 *t *t * p2.z +
              t*t*t*p3.z;

    return vec4(x,y,z,1);
}

vec3 cubicBernstein3(const vec3& p0, const vec3& p1,
                     const vec3& p2, const vec3& p3,
                     float t){
/*
    float t2 = t*t;
    float t3 = t*t*t;

    float B0 = 1 - 3*t + 3*t2 - t3;
    float B1 = 3*t - 6*t2 + 3*t3;
    float B2 = 3*t2 - 3*t3;
    float B3 = t3;
*/
    float x = pow(1-t, 3) * p0.x +
              pow(1-t, 2) * 3*t * p1.x +
              (1-t) * 3 *t *t * p2.x +
              t*t*t*p3.x;
    float y = pow(1-t, 3) * p0.y +
              pow(1-t, 2) * 3*t * p1.y +
              (1-t) * 3 *t *t * p2.y +
              t*t*t*p3.y;
    float z = pow(1-t, 3) * p0.z +
              pow(1-t, 2) * 3*t * p1.z +
              (1-t) * 3 *t *t * p2.z +
              t*t*t*p3.z;

    return vec3(x,y,z);
}

vec4 cubicBernsteinVector(float t){
    float t2 = t*t;
    float t3 = t*t*t;

    float B0 = 1 - 3*t + 3*t2 - t3;
    float B1 = 3*t - 6*t2 + 3*t3;
    float B2 = 3*t2 - 3*t3;
    float B3 = t3;

    return vec4(B0, B1, B2, B3);
}

glm::vec4 cubicBernsteinDerivative(float t){
    float t2 = t*t;

    float B0 = -3 + 6*t - 3*t2;
    float B1 = 3 - 12*t + 9*t2;
    float B2 = 6*t - 9*t2;
    float B3 = 3*t2;

    return vec4(B0, B1, B2, B3);
}

glm::vec4 cubicBernsteinSecondDerivative(float t){

    float B0 = 6 - 6*t;
    float B1 = -12 + 18*t;
    float B2 = 6 - 18*t;
    float B3 = 6*t;

    return vec4(B0, B1, B2, B3);
}