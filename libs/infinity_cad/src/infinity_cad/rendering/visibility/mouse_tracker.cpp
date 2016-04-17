//
// Created by jakub on 4/3/16.
//

#include <infinity_cad/rendering/visibility/mouse_tracker.h>

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

MouseTracker::MouseTracker(RayCast *rayCast, Renderer *renderer){
    this->rayCast = rayCast;
    this->renderer = renderer;
}

MouseTracker::~MouseTracker() {

}
//-----------------------//
//  PRIVATE
//-----------------------//

//-----------------------//
//  PUBLIC 
//-----------------------//


void MouseTracker::update(int x, int y) {
    float xGL = renderer->xPixelToGLCoord(x);
    float yGL = renderer->yPixelToGLCoord(y);

    rayCast->update(xGL, yGL,
                    renderer->getWindowWidth(),
                    renderer->getWindowHeight());

}
