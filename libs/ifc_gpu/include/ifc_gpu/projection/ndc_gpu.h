//
// Created by jakub on 4/17/16.
//

#ifndef IC_NDC_GPU_H
#define IC_NDC_GPU_H

#include <ifc_gpu/common.h>

__global__
void ndc_kernel(glm::vec4* d_vertices, int count, glm::mat4* d_MVP);

__host__
void ndc_init(glm::vec4* d_vertices, int count, glm::mat4* d_MVP);

__host__
void ndc_compute(const glm::vec4* vertices, int count,
                 const glm::mat4* MVP, glm::vec4** ndc_vertices);

#endif //IC_NDC_GPU_H
