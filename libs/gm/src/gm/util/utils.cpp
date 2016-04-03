//
// Created by jakub on 3/6/16.
//

#include <gm/util/utils.h>
#include <iostream>

using namespace glm;

void printMat4(const mat4& mat){
    std::string sep = ",    ";
    std::cout
    << mat[0].x << sep << mat[1].x << sep
    << mat[2].z << sep << mat[3].x << std::endl

    << mat[0].y << sep << mat[1].y << sep
    << mat[2].y << sep << mat[3].y << std::endl

    << mat[0].z << sep << mat[1].z << sep
    << mat[2].z << sep << mat[3].z << std::endl

    << mat[0].w << sep << mat[1].w << sep
    << mat[2].w << sep << mat[3].w << std::endl;

}

void printvec3(const vec3& v){
    std::string sep = ",    ";
    std::cout << "[" << v.x << ", " << v.y << ", " << v.z << "]" << std::endl;
}

void printvec4(const vec4& v){
    std::string sep = ",    ";
    std::cout
    << "[" << v.x << ", "
    << v.y << ", " << v.z << ", " << v.w
    << "]" << std::endl;
}

float gm::dot(const vec3& v1, const vec3& v2){
    float dotValue = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

    return dotValue;
}

float gm::euclideanDistance(const vec3& v1, const vec3& v2){
    float distance = 0;

    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;
    distance = dx*dx + dy*dy + dz*dz;

    distance = sqrt(distance);

    return distance;
}
