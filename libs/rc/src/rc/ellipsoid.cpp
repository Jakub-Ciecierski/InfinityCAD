//
// Created by jakub on 3/6/16.
//

#include <glm/detail/type_mat.hpp>
#include <rc/ellipsoid.h>
#include <rc/ray_constants.h>

using namespace glm;

Ellipsoid::Ellipsoid(float a, float b, float c) :
        a(a), b(b), c(c), color(1.0f, 0.5f, 0.0f){

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

float Ellipsoid::derivativeX(const glm::vec3& p) {
    float dx;
    dx = 2*p.x * D_MVP[0][0]
         + p.z * (D_MVP[0][2] + D_MVP[2][0])
         + p.y * (D_MVP[0][1] + D_MVP[1][0])
         + D_MVP[0][3] + D_MVP[3][0];
    return dx;
}

float Ellipsoid::derivativeY(const glm::vec3& p) {
    float dy;
    dy = 2*p.y * D_MVP[1][1]
         + p.z * (D_MVP[1][2] + D_MVP[2][1])
         + p.x * (D_MVP[0][1] + D_MVP[1][0])
         + D_MVP[1][3] + D_MVP[3][1];
    return dy;
}

float Ellipsoid::derivativeZ(const glm::vec3& p) {
    float dz;
    dz = 2*p.z * D_MVP[2][2]
         + p.x * (D_MVP[0][2] + D_MVP[2][0])
         + p.y * (D_MVP[1][2] + D_MVP[2][1])
         + D_MVP[2][3] + D_MVP[3][2];
    return dz;
}


//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

const Color &Ellipsoid::getColor() {
    return this->color;
}


void Ellipsoid::setARadius(float r) {
    a = r;
}

void Ellipsoid::setBRadius(float r) {
    b = r;
}

void Ellipsoid::setCRadius(float r) {
    c = r;
}

vec3 Ellipsoid::derivative(const glm::vec3& p) {
    vec3 d = vec3(derivativeX(p), derivativeY(p), derivativeZ(p));
    return d;
}

float Ellipsoid::intersect(float x, float y, const vec3& eye){
    float a = D_MVP[2][2];
    float b = x * (D_MVP[0][2] + D_MVP[2][0])
              + y*(D_MVP[1][2] + D_MVP[2][1])
              + D_MVP[3][2] + D_MVP[2][3];
    float c = (x*x * D_MVP[0][0]) + (x*y*D_MVP[0][1]) + (x*D_MVP[0][3]) +
              (y*x*D_MVP[1][0]) + (y*y * D_MVP[1][1]) + (y*D_MVP[1][3]) +
              (x*D_MVP[3][0]) + (y*D_MVP[3][1]) + D_MVP[3][3];

    if(isnan(a) == 1 || a == 0) return RAY_NO_SOLUTION; // a cant be zero
    float delta = sqrt((b*b) - (4*a*c));
    if(isnan(delta) == 1 || delta < 0) return RAY_NO_SOLUTION;

    float z1 = (-b + delta) / 2*a;
    float z2 = (-b - delta) / 2*a;

    float dist1 = sqrt((x - eye.x) * (x - eye.x) +
                       (y - eye.y) * (y - eye.y) +
                       (z1 - eye.z) * (z1 - eye.z));
    float dist2 = sqrt((x - eye.x) * (x - eye.x) +
                       (y - eye.y) * (y - eye.y) +
                       (z2 - eye.z) * (z2 - eye.z));

    float z = dist1 < dist2 ? z1 : z2;

/*
    std::cout << "delta: " << delta << std::endl;
    std::cout << "a:     " << a << std::endl;
    std::cout << "b:     " << b << std::endl;
    std::cout << "c:     " << c << std::endl;
    std::cout << "z1: " << z1 << ", z2: " << z2 << std::endl;
    std::cout << "z: " << z << std::endl;
    std::cout << std::endl;
*/
    return z;
}

void Ellipsoid::updateDMVPMatrix(const mat4& MVPMatrix) {
    mat4 D = mat4(1.0f);

    D[0].x = a;
    D[1].y = b;
    D[2].z = c;
    D[3].w = -1;

    mat4 MVP_Inverse = inverse(MVPMatrix);
    mat4 MVP_Inverse_Transposed = transpose(MVP_Inverse);

    D_MVP = MVP_Inverse_Transposed * D * MVP_Inverse;
}

