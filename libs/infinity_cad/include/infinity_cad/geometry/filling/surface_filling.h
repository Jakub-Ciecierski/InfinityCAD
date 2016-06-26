//
// Created by jakub on 6/25/16.
//

#ifndef IC_SURFACE_FILLING_H
#define IC_SURFACE_FILLING_H

#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

class SurfaceFilling {
private:
    Surface* surface1;
    Surface* surface2;
    Surface* surface3;

public:

    SurfaceFilling(Surface* surface1, Surface* surface2, Surface* surface3);

    ~SurfaceFilling();

    void start();
};


#endif //IC_SURFACE_FILLING_H
