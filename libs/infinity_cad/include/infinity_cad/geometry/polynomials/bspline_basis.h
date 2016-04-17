//
// Created by jakub on 4/12/16.
//

#ifndef IC_BSPLINE_BASIS_H
#define IC_BSPLINE_BASIS_H

#include <infinity_cad/rendering/render_objects/primitivies/point.h>

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

glm::vec4 computeBSpline(const std::vector<ifc::Point*>& deboorPoints,
                         const std::vector<float>& knotVector,
                         float t, int n);

float bsplineRecurive(float t, int n, int i,
                      const std::vector<float>& knotVector);

#endif //IC_BSPLINE_BASIS_H
