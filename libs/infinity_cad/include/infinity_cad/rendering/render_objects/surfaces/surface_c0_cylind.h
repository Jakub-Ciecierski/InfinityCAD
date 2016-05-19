//
// Created by jakub on 5/4/16.
//

#ifndef IC_SURFACE_C0_CYLIND_H
#define IC_SURFACE_C0_CYLIND_H


#include "surface_c0_rect.h"

class SurfaceC0Cylind : public Surface{
private:
    float radius;
    float height;

    Matrix<ifc::Point*> getMatrixFormOfAllPatches();

    void shapeTheCylidner(const Matrix<ifc::Point*>& rowWisePoints);

    /*
     * Creates a single patch with index [i,j] and of width and height
     */
    BicubicBezierPatch* createPatch(int i, int j,
                                    float patchHeight,
                                    const glm::vec3& origin);
    Matrix<ifc::Point*> initC0Points(int n, int m);

    void buildPatchesFromMatrix(const Matrix<ifc::Point*>& points);

protected:
    virtual void build() override;

public:
    SurfaceC0Cylind(SceneID id, std::string name,
                    int n, int m,
                    float radius, float height);
    SurfaceC0Cylind(SceneID id, std::string name,
                    Matrix<ifc::Point*> points);

    ~SurfaceC0Cylind();
};


#endif //IC_SURFACE_C0_CYLIND_H
