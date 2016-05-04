//
// Created by jakub on 5/3/16.
//

#ifndef IC_BEZIER_SURFACE_H
#define IC_BEZIER_SURFACE_H

#include <ifc_gpu/common.h>

namespace ifc_gpu{
    __global__
    void computeBezierSurfaceKernel(glm::mat4* d_xComponents,
                                    glm::mat4* d_yComponents,
                                    glm::mat4* d_zComponents,
                                    int patchCount,
                                    float u_min, float u_max,
                                    float v_min, float v_max,
                                    float du, float dv,
                                    glm::vec4* d_surfacePixels, int pixelCount,
                                    glm::mat4* d_MVP);

    __host__
    void computeBezierSurfaceInit(glm::mat4* d_xComponents,
                                  glm::mat4* d_yComponents,
                                  glm::mat4* d_zComponents,
                                  int patchCount,
                                  float u_min, float u_max,
                                  float v_min, float v_max,
                                  float du, float dv,
                                  glm::vec4* d_surfacePixels, int pixelCount,
                                  glm::mat4* d_MVP);

    __host__
    void computeBezierSurface(const glm::mat4* xComponents,
                              const glm::mat4* yComponents,
                              const glm::mat4* zComponents,
                              int patchCount,
                              float u_min, float u_max,
                              float v_min, float v_max,
                              float du, float dv,
                              glm::vec4* surfacePixels, int pixelCount,
                              const glm::mat4* VP);
}

#endif //IC_BEZIER_SURFACE_H
