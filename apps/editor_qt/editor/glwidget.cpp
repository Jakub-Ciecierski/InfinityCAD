#include "glwidget.h"
#include <drawer.h>
#include <iostream>

#include <geometry/projections/orthogonal_projection.h>
#include <geometry/projections/perspective_projection.h>

GLWidget::GLWidget(QWidget* parent) :
    QGLWidget(parent)
{
    width = 543;
    height = 449;

    camera = new Camera();
    orthogonalProjection = new OrthogonalProjection(1000.0f);
    perspectiveProjection = new PerspectiveProjection(width, height);

    renderableObjects.push_back(new Cube());

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);

    this->setFocusPolicy(Qt::ClickFocus);
}

GLWidget::~GLWidget(){
    delete camera;
    delete orthogonalProjection;
    delete perspectiveProjection;

    for(unsigned int i = 0; i < renderableObjects.size(); i++){
        delete renderableObjects[i];
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Left){
        camera->move(-0.1, 0, 0);
    }if(event->key() == Qt::Key_Right){
        camera->move(0.1, 0, 0);
    }

    if(event->key() == Qt::Key_Up){
        camera->move(0.0, 0.1, 0);
    }else if(event->key() == Qt::Key_Down){
        camera->move(0.0,-0.1, 0);
    }

    if(event->key() == Qt::Key_8){
        camera->rotate(-10.0, 0.0, 0);
    }else if(event->key() == Qt::Key_2){
        camera->rotate(10.0, 0.0, 0);
    }
    if(event->key() == Qt::Key_4){
        camera->rotate(0.0, -10.0, 0);
    }else if(event->key() == Qt::Key_6){
        camera->rotate(0.0, 10.0, 0);
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event){

}

void GLWidget::initializeGL() {
    glClearColor(0.2, 0.2, 0.2, 1);
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    camera->scale(0.5);
    const glm::mat4& viewMatrix = camera->getViewMatrix();
    const glm::mat4& projectionMatrix
            = orthogonalProjection->getProjectionMatrix();
    glm::mat4 VP = projectionMatrix * viewMatrix;

    for(unsigned int i = 0; i < renderableObjects.size(); i++){
        renderableObjects[i]->render(VP);
    }
    Torus torus(0.2, 0.1);
    torus.render(VP);

    // The axis, not to be translated
    drawLine();
}

void GLWidget::resizeGL(int width, int height){
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);
}
