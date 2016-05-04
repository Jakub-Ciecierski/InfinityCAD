//
// Created by jakub on 5/3/16.
//

#include <iostream>
#include "ifc_gpu/surfaces/bezier_surface_gpu.h"

using namespace glm;

__device__
float ifc_gpu::getMultplicationValueGPU(const vec4& v1, const mat4& m, const vec4& v2){
    vec4 res = v1 * m * v2;
    float val = res.x + res.y + res.z + res.w;

    return val;
}

__device__
vec4 ifc_gpu::cubicBernsteinVectorGPU(float t){
    float t2 = t*t;
    float t3 = t*t*t;

    float B0 = 1 - 3*t + 3*t2 - t3;
    float B1 = 3*t - 6*t2 + 3*t3;
    float B2 = 3*t2 - 3*t3;
    float B3 = t3;

    return vec4(B0, B1, B2, B3);
}

__global__
void ifc_gpu::computeBezierSurfaceKernel(glm::mat4* d_xComponents,
                                         glm::mat4* d_yComponents,
                                         glm::mat4* d_zComponents,
                                         int patchCount,
                                         glm::vec2* uvPatchParameters,
                                         int paramCount,
                                         glm::vec4* d_surfacePixels, int pixelCount,
                                         glm::mat4* d_MVP){
    int pixelsPerPatch = pixelCount / patchCount;

    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if(tid < pixelCount){
        int patchID = tid / pixelsPerPatch;
        if(patchID < patchCount){
            int patchPixel = tid % pixelsPerPatch;

            vec2 params = uvPatchParameters[patchPixel];

            float u =  params.x;
            float v =  params.y;

            vec4 Bu = cubicBernsteinVectorGPU(u);
            vec4 Bv = cubicBernsteinVectorGPU(v);

            float x = getMultplicationValueGPU(Bu, d_xComponents[patchID], Bv);
            float y = getMultplicationValueGPU(Bu, d_yComponents[patchID], Bv);
            float z = getMultplicationValueGPU(Bu, d_zComponents[patchID], Bv);

            vec4 point(x, y, z, 1);
            point = *d_MVP * point;

            point.x /= point.w;
            point.y /= point.w;

            d_surfacePixels[tid] = point;
        }

    }
}

__host__
void ifc_gpu::computeBezierSurfaceInit(glm::mat4* d_xComponents,
                                       glm::mat4* d_yComponents,
                                       glm::mat4* d_zComponents,
                                       int patchCount,
                                       glm::vec2* uvPatchParameters,
                                       int paramCount,
                                       glm::vec4* d_surfacePixels,
                                       int pixelCount,
                                       glm::mat4* d_MVP){
    dim3 DimGrid((pixelCount / MAX_BLOCK_THREAD_COUNT) + 1, 1, 1);
    dim3 DimBlock(MAX_BLOCK_THREAD_COUNT, 1, 1);

    computeBezierSurfaceKernel << <DimGrid, DimBlock>> >
                                                (d_xComponents, d_yComponents,
                                                        d_zComponents,
                                                        patchCount,
                                                        uvPatchParameters,
                                                        paramCount,
                                                        d_surfacePixels,
                                                        pixelCount,
                                                        d_MVP);
    cudaDeviceSynchronize();
}

__host__
void ifc_gpu::computeBezierSurface(mat4* xComponents,
                                   mat4* yComponents,
                                   mat4* zComponents,
                                   int patchCount,
                                   glm::vec2* uvPatchParameters,
                                   int paramCount,
                                   vec4* surfacePixels, int pixelCount,
                                   const mat4* MVP){
    size_t pointComponentsMemSize = patchCount*sizeof(mat4);
    size_t surfacePixelsMemSize = pixelCount * sizeof(vec4);
    size_t uvPatchParametersMemSize = paramCount * sizeof(vec2);
    size_t mvpMemSize = sizeof(mat4);

    mat4* d_xComponents;
    mat4* d_yComponents;
    mat4* d_zComponents;
    vec2* d_uvPatchParameters;
    vec4* d_surfacePixels;
    mat4* d_MVP;

    cudaError_t err;

    if ((err = cudaMalloc((void**)&d_MVP, mvpMemSize)) != cudaSuccess)
        C_ERR(err);

    if ((err = cudaMalloc((void**)&d_xComponents, pointComponentsMemSize)) != cudaSuccess)
        C_ERR(err);

    if ((err = cudaMalloc((void**)&d_yComponents, pointComponentsMemSize)) != cudaSuccess)
        C_ERR(err);

    if ((err = cudaMalloc((void**)&d_zComponents, pointComponentsMemSize)) != cudaSuccess)
        C_ERR(err);

    if ((err = cudaMalloc((void**)&d_uvPatchParameters, uvPatchParametersMemSize)) != cudaSuccess)
        C_ERR(err);

    if ((err = cudaMalloc((void**)&d_surfacePixels, surfacePixelsMemSize)) != cudaSuccess)
        C_ERR(err);


    if ((err = cudaMemcpy(d_xComponents, xComponents, pointComponentsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_yComponents, yComponents, pointComponentsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_zComponents, zComponents, pointComponentsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_uvPatchParameters, uvPatchParameters, uvPatchParametersMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_MVP, MVP, mvpMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);


    computeBezierSurfaceInit(d_xComponents, d_yComponents, d_zComponents,
                             patchCount,
                             d_uvPatchParameters, paramCount,
                             d_surfacePixels,
                             pixelCount, d_MVP);

    if ((err = cudaMemcpy(surfacePixels, d_surfacePixels, surfacePixelsMemSize,
                          cudaMemcpyDeviceToHost)) != cudaSuccess) C_ERR(err);

    if ((err = cudaFree(d_xComponents)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_yComponents)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_zComponents)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_uvPatchParameters)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_surfacePixels)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_MVP)) != cudaSuccess) C_ERR(err);
}