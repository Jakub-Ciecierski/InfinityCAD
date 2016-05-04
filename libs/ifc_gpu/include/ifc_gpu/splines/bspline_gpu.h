//
// Created by jakub on 4/17/16.
//

#ifndef IC_BSPLINE_GPU_H
#define IC_BSPLINE_GPU_H

#include <ifc_gpu/common.h>

namespace ifc_gpu{

    __device__
    float bsplineRecursiveGPU(float t, int n, int i,
                              float* knotVector);

    __global__
    void computeBSplineKernel(glm::vec3* points, int pointCount,
                              float* knotVector,
                              glm::vec4* outCurve, int outSize,
                              float tstart, float dt, int degree,
                              glm::mat4* MVP);
    __host__
    void computeBSplineInit(glm::vec3* d_points, int pointCount,
                            float* d_knotVector,
                            glm::vec4* d_outCurve, int outSize,
                            float tstart, float dt, int degree,
                            glm::mat4* MVP);

    __host__
    void computeBSpline(glm::vec3* points, int pointCount,
                        float* knotVector, int knotCount,
                        glm::vec4* outCurve, int outSize,
                        float tstart, float dt, int degree,
                        const glm::mat4* MVP);

}

#endif //IC_BSPLINE_GPU_H
