#include "glwidget.h"

GLWidget::GLWidget(QWidget* parent) :
    QGLWidget(parent)
{

}

void GLWidget::initializeGL() {
    glClearColor(0.2, 0.2, 0.2, 1);
}

void GLWidget::paintGL(){
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

void GLWidget::resizeGL(int width, int height){
    glViewport(0,0,width, height);
}
