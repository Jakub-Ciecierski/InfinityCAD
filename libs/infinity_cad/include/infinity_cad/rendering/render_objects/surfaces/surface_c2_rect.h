//
// Created by jakub on 5/15/16.
//

#ifndef IC_SURFACE_C2_RECT_H
#define IC_SURFACE_C2_RECT_H


#include "infinity_cad/rendering/render_objects/surfaces/surface.h"

class SurfaceC2Rect : public Surface {
private:
    Matrix<ifc::Point*>* deboorPoints;

    float width;
    float height;

protected:
    virtual void build() override;

    virtual void drawPolygon(const glm::mat4& VP, int segments = 1) override;
public:

    SurfaceC2Rect(SceneID id, std::string name,
                  int n, int m,
                  float width, float height);
    SurfaceC2Rect(SceneID id, std::string name,
                  Matrix<ifc::Point*> points);

    ~SurfaceC2Rect();

    virtual glm::vec3 compute(float u, float v) override;

    virtual glm::vec3 computeDu(float u, float v) override;
    virtual glm::vec3 computeDuu(float u, float v) override;
    virtual glm::vec3 computeDuv(float u, float v) override;

    virtual glm::vec3 computeDv(float u, float v) override;
    virtual glm::vec3 computeDvv(float u, float v) override;
    virtual glm::vec3 computeDvu(float u, float v) override;

    virtual bool replacePoint(ifc::Point* src, ifc::Point* dest) override;

    virtual void update() override;

};


#endif //IC_SURFACE_C2_RECT_H
