//
// Created by jakub on 5/2/16.
//

#ifndef IC_SURFACE_RECT_C0_H
#define IC_SURFACE_RECT_C0_H

#include <infinity_cad/rendering/render_objects/surfaces/surface.h>
#include <math/matrix.h>
#include <infinity_cad/geometry/surfaces/bicubic_bezier_patch.h>

class SurfaceC0Rect : public Surface{
private:
    float width;
    float height;

    void buildPatchesFromMatrix(const Matrix<ifc::Point*>& points);

    Matrix<ifc::Point*> getMatrixFormOfAllPatches();

    /*
     * Creates a single patch with index [i,j] and of width and height
     */
    BicubicBezierPatch* createPatch(int i, int j,
                                   float patchWidth, float patchHeight,
                                   const glm::vec3& origin);
    Matrix<ifc::Point*> initC0Points(int n, int m);

protected:
    virtual void build() override;


public:
    SurfaceC0Rect(SceneID id, std::string name,
                  int n, int m,
                  float width, float height);
    SurfaceC0Rect(SceneID id, std::string name,
                  Matrix<ifc::Point*> points);

    ~SurfaceC0Rect();
};


#endif //IC_SURFACE_RECT_C0_H
