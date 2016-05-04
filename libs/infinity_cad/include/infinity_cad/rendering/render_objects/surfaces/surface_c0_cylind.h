//
// Created by jakub on 5/4/16.
//

#ifndef IC_SURFACE_C0_CYLIND_H
#define IC_SURFACE_C0_CYLIND_H


#include "surface_rect_c0.h"

class SurfaceC0Cylind : public Surface{
private:
    const int CUBIC_COUNT = 4;

    float radius;
    float height;

    Matrix<BicubicBezierPatch*> patches;

    int MAX_PIXEL_COUNT = 100000;
    glm::vec4* surfacePixels;

    std::vector<ifc::Point*> allPoints;

    Matrix<ifc::Point*> getMatrixFormOfAllPatches();

    void shapeTheCylidner(const Matrix<ifc::Point*>& rowWisePoints);

    /*
     * Creates a single patch with index [i,j] and of width and height
     */
    BicubicBezierPatch* createPatch(int i, int j,
                                    float patchHeight,
                                    const glm::vec3& origin);
    Matrix<ifc::Point*> initC0Points(int n, int m);
    std::string createPointName(int patchN, int patchM,
                                int pointI, int pointJ);

    void drawCPU(const glm::mat4& VP, const Color& color,
                 float u_min, float u_max,
                 float v_min, float v_max,
                 float du, float dv);

    void drawGPU(const glm::mat4& VP, const Color& color,
                 float u_min, float u_max,
                 float v_min, float v_max,
                 float du, float dv);

    void drawPatch(const BicubicBezierPatch* patch,
                   const glm::mat4& VP,
                   float u_min, float u_max,
                   float v_min, float v_max,
                   float du, float dv);
protected:
    virtual void draw(const glm::mat4& VP, const Color& color) override;
    virtual void build() override;


public:
    SurfaceC0Cylind(SceneID id, std::string name,
                    int n, int m,
                    float radius, float height);

    ~SurfaceC0Cylind();

    virtual void update() override;

    const std::vector<ifc::Point*>& getAllPoints();

};


#endif //IC_SURFACE_C0_CYLIND_H
