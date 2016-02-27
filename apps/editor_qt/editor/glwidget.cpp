#include "glwidget.h"
#include <drawer.h>
#include <iostream>

GLWidget::GLWidget(QWidget* parent) :
    QGLWidget(parent)
{
    width = 0;
    height = 0;

    torus = new Torus(0.6, 0.2);
}

void GLWidget::initializeGL() {
    glClearColor(0.2, 0.2, 0.2, 1);
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);

    torus->render(glm::mat4());

    // The axis, not to be translated
    drawLine();
}

void GLWidget::resizeGL(int width, int height){
    this->width = width;
    this->height = height;

    std::cout << "Width: " << this->width << std::endl;
    std::cout << "Height: " << this->height << std::endl;

    glViewport(0,0,width, height);
}
