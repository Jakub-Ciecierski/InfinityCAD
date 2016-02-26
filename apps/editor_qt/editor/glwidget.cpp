#include "glwidget.h"
#include <drawer.h>

GLWidget::GLWidget(QWidget* parent) :
    QGLWidget(parent)
{

}

void GLWidget::initializeGL() {
    glClearColor(0.2, 0.2, 0.2, 1);
}

void GLWidget::paintGL(){
   drawLine();
}

void GLWidget::resizeGL(int width, int height){
    glViewport(0,0,width, height);
}
