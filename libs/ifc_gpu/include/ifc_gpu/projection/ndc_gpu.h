//
// Created by jakub on 4/17/16.
//

#ifndef IC_NDC_GPU_H
#define IC_NDC_GPU_H

#define CUDA_VERSION 7000
#define GLM_FORCE_CUDA

#include <glm/glm.hpp>
#include <stdio.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
#define C_ERR(err) (printf("%s in %s at line %d\n",\
                    cudaGetErrorString(err), __FILE__, __LINE__),\
                    exit(EXIT_FAILURE))

#define MAX_GRID_DIM_SIZE 65535
#define MAX_BLOCK_THREAD_COUNT 512
#define MAX_BLOCK_DIM_SIZE 32

__global__
void ndc_kernel(glm::vec4* d_vertices, int count, glm::mat4* d_MVP);

__host__
void ndc_init(glm::vec4* d_vertices, int count, glm::mat4* d_MVP);

__host__
void ndc_compute(const glm::vec4* vertices, int count,
                 const glm::mat4* MVP, glm::vec4** ndc_vertices);
/*
#ifdef __cplusplus
}
#endif // extern
*/
#endif //IC_NDC_GPU_H
