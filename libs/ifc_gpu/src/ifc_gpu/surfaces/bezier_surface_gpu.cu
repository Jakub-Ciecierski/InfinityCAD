//
// Created by jakub on 5/3/16.
//

#include "ifc_gpu/surfaces/bezier_surface_gpu.h"

using namespace glm;

__global__
void ifc_gpu::computeBezierSurfaceKernel(glm::mat4* d_xComponents,
                                         glm::mat4* d_yComponents,
                                         glm::mat4* d_zComponents,
                                         int patchCount,
                                         float u_min, float u_max,
                                         float v_min,  float v_max,
                                         float du, float dv,
                                         glm::vec4* d_surfacePixels, int pixelCount,
                                         glm::mat4* d_MVP){
    int pixelsPerPatch = pixelCount / patchCount;

    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if(tid < pixelCount){
        int patchID = tid / pixelsPerPatch;
        int patchPixel = tid % pixelsPerPatch;

        float u;
        //float t = tstart + tid*dt;
    }
}

__host__
void ifc_gpu::computeBezierSurfaceInit(glm::mat4* d_xComponents,
                                       glm::mat4* d_yComponents,
                                       glm::mat4* d_zComponents,
                                       int patchCount,
                                       float u_min, float u_max,
                                       float v_min,  float v_max,
                                       float du, float dv,
                                       glm::vec4* d_surfacePixels,
                                       int pixelCount,
                                       glm::mat4* d_MVP){
    dim3 DimGrid((pixelCount / MAX_BLOCK_THREAD_COUNT) + 1, 1, 1);
    dim3 DimBlock(MAX_BLOCK_THREAD_COUNT, 1, 1);

    computeBezierSurfaceKernel << <DimGrid, DimBlock>> >
                                                (d_xComponents, d_yComponents,
                                                        d_zComponents,
                                                        patchCount,
                                                        u_min, u_max,
                                                        v_min, v_max,
                                                        du, dv,
                                                        d_surfacePixels,
                                                        pixelCount,
                                                        d_MVP);
    cudaDeviceSynchronize();
}

void ifc_gpu::computeBezierSurface(const mat4* xComponents,
                                   const mat4* yComponents,
                                   const mat4* zComponents,
                                   int patchCount,
                                   float u_min, float u_max,
                                   float v_min, float v_max,
                                   float du, float dv,
                                   vec4* surfacePixels, int pixelCount,
                                   const mat4* MVP){
    int pointComponentsMemSize = patchCount*sizeof(mat4);
    int surfacePixelsMemSize = pixelCount * sizeof(vec4);
    int mvpMemSize = sizeof(mat4);

    mat4* d_xComponents;
    mat4* d_yComponents;
    mat4* d_zComponents;
    vec4* d_surfacePixels;
    mat4* d_MVP;

    cudaError_t err;

    if ((err = cudaMalloc((void**)&d_xComponents, pointComponentsMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&d_yComponents, pointComponentsMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&d_zComponents, pointComponentsMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&d_surfacePixels, surfacePixelsMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&mvpMemSize, mvpMemSize)) != cudaSuccess)
        C_ERR(err);


    if ((err = cudaMemcpy(d_xComponents, xComponents, pointComponentsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_yComponents, yComponents, pointComponentsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_zComponents, zComponents, pointComponentsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_MVP, MVP, mvpMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);

    // go

    computeBezierSurfaceInit(d_xComponents, d_yComponents, d_zComponents,
    patchCount, u_min, u_max, v_min, v_max, du, dv, d_surfacePixels,
                             pixelCount, d_MVP);

    if ((err = cudaMemcpy(d_surfacePixels, surfacePixels, surfacePixelsMemSize,
                          cudaMemcpyDeviceToHost)) != cudaSuccess) C_ERR(err);

    if ((err = cudaFree(d_xComponents)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_yComponents)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_zComponents)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_surfacePixels)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_MVP)) != cudaSuccess) C_ERR(err);
}
