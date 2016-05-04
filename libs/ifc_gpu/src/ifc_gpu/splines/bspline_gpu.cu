//
// Created by jakub on 4/17/16.
//

#include <iostream>
#include "ifc_gpu/splines/bspline_gpu.h"

using namespace glm;
using namespace ifc_gpu;

__device__
float ifc_gpu::bsplineRecursiveGPU(float t, int n, int i,
                          float* knotVector){
    if (n == 0){
        if(t >= knotVector[i] && t < knotVector[i+1])
            return 1;
        else
            return 0;
    }
    float leftRecursion = (t - knotVector[i]);
    float leftDenominator = (knotVector[i+n] - knotVector[i]);
    if(leftRecursion == 0 || leftDenominator == 0)
        leftRecursion = 1;
    else
        leftRecursion /= leftDenominator;

    float rightRecursion = knotVector[i+1+n] - t;
    float rightDenominator = (knotVector[i+1+n] - knotVector[i+1]);
    if(rightRecursion == 0 || rightDenominator == 0)
        rightRecursion = 1;
    else
        rightRecursion /= rightDenominator;

    leftRecursion *= bsplineRecursiveGPU(t, n-1, i, knotVector);
    rightRecursion *= bsplineRecursiveGPU(t, n-1, i+1, knotVector);

    return leftRecursion + rightRecursion;
}

__global__
void ifc_gpu::computeBSplineKernel(vec3* points, int pointCount,
                                   float* knotVector,
                                   vec4* outCurve, int outSize,
                                   float tstart, float dt, int degree,
                                   mat4* MVP){
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if(tid < outSize){
        float x,y,z;
        x = y = z = 0;
        float t = tstart + tid*dt;
        for(int i = 0;i < pointCount;i++){
            vec3& p = points[i];

            float bsplineBasisValue = bsplineRecursiveGPU(t, degree, i,
                                                          knotVector);
            x += p.x * bsplineBasisValue;
            y += p.y * bsplineBasisValue;
            z += p.z * bsplineBasisValue;
        }

        vec4 v = vec4(x,y,z,1);
        v = (*MVP) * v;

        v.x /= v.w;
        v.y /= v.w;

        outCurve[tid] = v;
    }
}

__host__
void ifc_gpu::computeBSplineInit(glm::vec3* d_points, int pointCount,
                        float* d_knotVector,
                        glm::vec4* d_outCurve, int outSize,
                        float tstart, float dt, int degree,
                        glm::mat4* MVP){
    dim3 DimGrid((outSize / MAX_BLOCK_THREAD_COUNT) + 1, 1, 1);
    dim3 DimBlock(MAX_BLOCK_THREAD_COUNT, 1, 1);

    computeBSplineKernel << <DimGrid, DimBlock>> >
                    (
                            d_points, pointCount, d_knotVector,
                            d_outCurve, outSize,
                            tstart, dt, degree, MVP);

    cudaDeviceSynchronize();
}

__host__
void ifc_gpu::computeBSpline(vec3* points, int pointCount,
                    float* knotVector, int knotCount,
                    vec4* outCurve, int outSize,
                    float tstart, float dt, int degree,
                    const mat4* MVP){
    int pointsMemSize = pointCount * sizeof(vec3);
    int knotsMemSize = knotCount * sizeof(float);
    int outputMemSize = outSize * sizeof(vec4);
    int mvpMemSize = sizeof(mat4);

    vec3* d_points;
    float* d_knotVector;
    vec4* d_outCurve;
    mat4* d_MVP;

    cudaError_t err;

    if ((err = cudaMalloc((void**)&d_points, pointsMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&d_knotVector, knotsMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&d_outCurve, outputMemSize)) != cudaSuccess)
        C_ERR(err);
    if ((err = cudaMalloc((void**)&d_MVP, mvpMemSize)) != cudaSuccess)
        C_ERR(err);

    if ((err = cudaMemcpy(d_points, points, pointsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_knotVector, knotVector, knotsMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_outCurve, outCurve, outSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMemcpy(d_MVP, MVP, mvpMemSize,
                          cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);

    computeBSplineInit(d_points, pointCount, d_knotVector,
                       d_outCurve, outSize, tstart, dt, degree, d_MVP);

    if ((err = cudaMemcpy(outCurve, d_outCurve, outputMemSize,
                          cudaMemcpyDeviceToHost)) != cudaSuccess) C_ERR(err);

    if ((err = cudaFree(d_points)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_knotVector)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_outCurve)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_MVP)) != cudaSuccess) C_ERR(err);
}