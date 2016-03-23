//
// Created by jakub on 3/6/16.
//

#ifndef MG1_UTILS_H
#define MG1_UTILS_H

#include <glm/gtc/matrix_transform.hpp>

void printMat4(const glm::mat4& mat);

void printvec3(const glm::vec3& v);

void printvec4(const glm::vec4& v);

namespace gm {
    float dot(const glm::vec3 &v1, const glm::vec3 &v2);
}

#endif //MG1_UTILS_H
