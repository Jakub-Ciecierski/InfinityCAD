//
// Created by jakub on 5/3/16.
//

#ifndef IC_BEZIER_SURFACE_H
#define IC_BEZIER_SURFACE_H

#include <ifc_gpu/common.h>

namespace ifc_gpu{

    __device__
    float getMultplicationValueGPU(const glm::vec4& v1,
                                const glm::mat4& m,
                                const glm::vec4& v2);

    __device__
    glm::vec4 cubicBernsteinVectorGPU(float t);

    __global__
    void computeBezierSurfaceKernel(glm::mat4* d_xComponents,
                                    glm::mat4* d_yComponents,
                                    glm::mat4* d_zComponents,
                                    int patchCount,
                                    glm::vec2* uvPatchParameters,
                                    int paramCount,
                                    glm::vec4* d_surfacePixels, int pixelCount,
                                    glm::mat4* d_MVP);

    __host__
    void computeBezierSurfaceInit(glm::mat4* d_xComponents,
                                  glm::mat4* d_yComponents,
                                  glm::mat4* d_zComponents,
                                  int patchCount,
                                  glm::vec2* uvPatchParameters,
                                  int paramCount,
                                  glm::vec4* d_surfacePixels, int pixelCount,
                                  glm::mat4* d_MVP);

    __host__
    void computeBezierSurface(glm::mat4* xComponents,
                              glm::mat4* yComponents,
                              glm::mat4* zComponents,
                              int patchCount,
                              glm::vec2* uvPatchParameters,
                              int paramCount,
                              glm::vec4* surfacePixels, int pixelCount,
                              const glm::mat4* VP);
}

#endif //IC_BEZIER_SURFACE_H
