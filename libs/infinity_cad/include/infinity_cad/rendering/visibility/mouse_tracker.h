//
// Created by jakub on 4/3/16.
//

#ifndef MG1_MOUSE_TRACKER_H
#define MG1_MOUSE_TRACKER_H

#include <infinity_cad/rendering/renderer.h>
#include "infinity_cad/rendering/visibility/ray_cast.h"

class MouseTracker {
private:
    Renderer* renderer;
    RayCast* rayCast;

public:

    MouseTracker(RayCast* rayCast, Renderer*);

    ~MouseTracker();

    void update(int x, int y);
};


#endif //MG1_MOUSE_TRACKER_H
