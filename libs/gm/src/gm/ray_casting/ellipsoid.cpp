//
// Created by jakub on 3/6/16.
//

#include <glm/detail/type_mat.hpp>
#include <gm/ray_casting/ellipsoid.h>
#include <gm/ray_casting/ray_constants.h>
#include <iostream>
#include <gm/utils.h>

using namespace glm;

Ellipsoid::Ellipsoid(float a, float b, float c) : a(a), b(b), c(c){

}

void Ellipsoid::updateDMVPMatrix(const mat4& MVPMatrix) {
    mat4 D = mat4(1.0f);

    D[0].x = a;
    D[1].y = b;
    D[2].z = c;
    D[3].w = -1;

    //mat4 MVP_Inverse = inverse(MVPMatrix);
    mat4 MVP_Inverse = MVPMatrix;
    mat4 MVP_Inverse_Transposed = glm::transpose(MVP_Inverse);

    D_MVP = MVP_Inverse_Transposed * D * MVP_Inverse;

/*
    std::cout << "D:" << std::endl;
    printMat4(D);


    std::cout << "MVPMatrix: " << std::endl;
    printMat4(MVPMatrix);

    std::cout << "MVP_Inverse: " << std::endl;
    printMat4(MVP_Inverse);

    std::cout << "MVP_Inverse_Transposed: " << std::endl;
    printMat4(MVP_Inverse_Transposed);

    std::cout << "D_MVP" << std::endl;
    printMat4(D_MVP);
    */
}


float Ellipsoid::intersect(float x, float y,
                          const mat4& MVPMatrix) {
    updateDMVPMatrix(MVPMatrix);

    // Solve for z: (w^T * D_MVP * w), where w = [x, y, z, 1]^T
    // After appropriate algebra we get equation:
    // az^2 + bz + c = 0
    float a = D_MVP[2][2];
    float b = (x * D_MVP[0][2] + y * D_MVP[1][2] + D_MVP[3][2]) +
              (x * D_MVP[2][0] + y * D_MVP[2][1] + D_MVP[2][3]);
    float c = (x*x * D_MVP[0][0]) + (x*y*D_MVP[0][1]) + (y*D_MVP[0][3]) +
              (y*x*D_MVP[1][0]) + (y*y * D_MVP[1][1]) + (y*D_MVP[1][3]) +
              (x*D_MVP[3][0]) + (y*D_MVP[3][1]) + D_MVP[3][3];

    if(a == 0) RAY_NO_SOLUTION; // a cant be zero

    float delta = sqrt((b*b) - (4*a*c));
    if(delta < 0) RAY_NO_SOLUTION; // no real solution

    float z1 = (-b + delta) / 2*a;
    float z2 = (-b - delta) / 2*a;

    /*
    std::cout << "delta: " << delta << std::endl;
    std::cout << "a:     " << a << std::endl;
    std::cout << "b:     " << b << std::endl;
    std::cout << "c:     " << c << std::endl;
    std::cout << "z1: " << z1 << ", z2: " << z2 << std::endl;
    std::cout << std::endl;
     */
    // choose the closest
    float z = z1 > z2 ? z1 : z2;

    return z;
}
