//
// Created by jakub on 4/6/16.
//

#ifndef IC_BERNSTEIN_H
#define IC_BERNSTEIN_H

#include <glm/gtc/matrix_transform.hpp>

glm::vec4 linearBernstein(const glm::vec3&p0, const glm::vec3&p1,
                          float t);

glm::vec4 quadraticBernstein(const glm::vec3&p0, const glm::vec3&p1,
                             const glm::vec3&p2,
                             float t);
glm::vec3 quadraticBernstein3(const glm::vec3& p0, const glm::vec3& p1,
                              const glm::vec3& p2, float t);

glm::vec4 cubicBernstein(const glm::vec3& p1, const glm::vec3& p2,
                         const glm::vec3& p3, const glm::vec3& p4,
                         float t);
glm::vec3 cubicBernstein3(const glm::vec3& p0, const glm::vec3& p1,
                          const glm::vec3& p2, const glm::vec3& p3,
                          float t);
glm::vec4 cubicBernsteinVector(float t);
glm::vec4 cubicBernsteinDerivative(float t);
glm::vec4 cubicBernsteinSecondDerivative(float t);

#endif //IC_BERNSTEIN_H
