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

    ~SurfaceC2Rect();

    virtual void update() override;

};


#endif //IC_SURFACE_C2_RECT_H
