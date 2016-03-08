//
// Created by jakub on 3/6/16.
//

#include <gm/utils.h>
#include <iostream>

using namespace glm;

void printMat4(const mat4& mat){
    std::string sep = ",    ";
    std::cout
    << mat[0].x << sep << mat[0].y << sep
    << mat[0].z << sep << mat[0].w << std::endl

    << mat[1].x << sep << mat[1].y << sep
    << mat[1].z << sep << mat[1].w << std::endl

    << mat[2].x << sep << mat[2].y << sep
    << mat[2].z << sep << mat[2].w << std::endl

    << mat[3].x << sep << mat[3].y << sep
    << mat[3].z << sep << mat[3].w << std::endl;
}

void printvec3(const vec3& v){
    std::string sep = ",    ";
    std::cout << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}
