//
// Created by jakub on 4/17/16.
//

#ifndef IC_COMMON_H
#define IC_COMMON_H

#define CUDA_VERSION 7000
#define GLM_FORCE_CUDA

#include <glm/glm.hpp>
#include <stdio.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define C_ERR(err) (printf("%s in %s at line %d\n",\
                    cudaGetErrorString(err), __FILE__, __LINE__),\
                    exit(EXIT_FAILURE))

#define MAX_GRID_DIM_SIZE 65535
#define MAX_BLOCK_THREAD_COUNT 512
#define MAX_BLOCK_DIM_SIZE 32

#endif //IC_COMMON_H
