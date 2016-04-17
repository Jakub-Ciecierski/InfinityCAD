//
// Created by jakub on 4/17/16.
//

#include <ifc_gpu/projection/ndc_gpu.h>
#include <iostream>

using namespace glm;

__global__
void ndc_kernel(vec4* d_vertices, int count, mat4* d_MVP){
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < count){
        vec4& v = d_vertices[tid];

        v = (*d_MVP) * v;

        v.x /= v.w;
        v.y /= v.w;
        v.z /= v.w;
    }
}

__host__
void ndc_init(vec4* d_vertices, int count, mat4* d_MVP){
    dim3 DimGrid((count / MAX_BLOCK_THREAD_COUNT) + 1, 1, 1);
    dim3 DimBlock(MAX_BLOCK_THREAD_COUNT, 1, 1);

    ndc_kernel << <DimGrid, DimBlock>> >(d_vertices, count, d_MVP);

    cudaDeviceSynchronize();
}

__host__
void ndc_compute(const vec4* vertices, int count,
                 const mat4* MVP, vec4** ndc_vertices){
    //std::cout << "ndc_compute" << std::endl;
    int verticesMemSize = count * sizeof(vec4);
    int mvpMemSize = sizeof(mat4);

    vec4* d_vertices;
    mat4* d_MVP;

    cudaError_t err;

    if ((err = cudaMalloc((void**)&d_vertices, verticesMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&d_MVP, mvpMemSize)) != cudaSuccess)
        C_ERR(err);

    if ((err = cudaMemcpy(d_vertices, vertices, verticesMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_MVP, MVP, mvpMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);

    ndc_init(d_vertices, count, d_MVP);

    if ((err = cudaMemcpy(*ndc_vertices, d_vertices, verticesMemSize,
                          cudaMemcpyDeviceToHost)) != cudaSuccess) C_ERR(err);

    if ((err = cudaFree(d_vertices)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_MVP)) != cudaSuccess) C_ERR(err);
}