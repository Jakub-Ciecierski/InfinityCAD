//
// Created by jakub on 4/12/16.
//

#ifndef IC_BSPLINE_BASIS_H
#define IC_BSPLINE_BASIS_H

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <gm/rendering/render_bodies/primitivies/point.h>

glm::vec4 computeBSpline(const std::vector<ic::Point*>& deboorPoints,
                         const std::vector<float>& knotVector,
                         float t, int n);

float bsplineRecurive(float t, int n, int i,
                      const std::vector<float>& knotVector);

#endif //IC_BSPLINE_BASIS_H
