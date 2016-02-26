//
// Created by jakub on 2/26/16.
//

#include "drawer.h"
#include <GL/gl.h>

void drawLine() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(-1.0, 1.0, 0.0);
    glVertex3f(1.0, 1.0, 0.0);

    glVertex3f(1.0, 1.0, 0.0);
    glVertex3f(1.0, -1.0, 0.0);

    glVertex3f(1.0, -1.0, 0.0);
    glVertex3f(-1.0, -1.0, 0.0);

    glVertex3f(-0.9, -0.9, 0.0);
    glVertex3f(-0.9, 0.9, 0.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();
}

void drawTorus(const Torus &torus) {
    glClear(GL_COLOR_BUFFER_BIT);

    int centerPointsCount = 360;
    int insidePointsCount = 360;

    glBegin(GL_LINES);

    for(int i = 0; i < centerPointsCount; i++){
        for(int j = 0; j < insidePointsCount; j++){

        }
    }

    glEnd();
}
