//
// Created by jakub on 3/6/16.
//

#ifndef MG1_RAY_H
#define MG1_RAY_H

#include <gm/rendering/renderer.h>
#include <gm/ray_casting/ellipsoid.h>

class Ray {
private:

public:
    Ellipsoid* ellipsoid;

    Ray();

    void rayCasting(Renderer& renderer);
};


#endif //MG1_RAY_H
