//
// Created by jakub on 3/6/16.
//

#include <gm/ray_casting/ray.h>
#include <GL/gl.h>
#include <gm/ray_casting/ray_constants.h>

Ray::Ray() {

}

void Ray::rayCasting(Renderer& renderer) {
    Scene* scene = renderer.getScene();
    unsigned int width = renderer.getWindowWidth();
    unsigned int height = renderer.getWindowHeight();

    glBegin(GL_POINTS);
    for(unsigned int x = 0; x < width; x++){
        float xGL = renderer.xPixelToGLCoord(x);
        for(unsigned int y = 0; y < height; y++){
            float yGL = renderer.yPixelToGLCoord(y);

            float z = ellipsoid->intersect(xGL, yGL,
                                           scene->getMVP());

            if(z != RAY_NO_SOLUTION)
                glColor3f(1.0, 0.5, 0);
            else {
                Color c = scene->getColor();
                glColor3f(c.R, c.G, c.B);
            }

            glVertex2f(xGL,
                       yGL);
        }
    }
    glEnd();
}
