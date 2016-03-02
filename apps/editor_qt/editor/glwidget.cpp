#include "mainwindow.h"
#include "glwidget.h"
#include <drawer.h>
#include <iostream>

#include <geometry/projections/perspective_projection.h>

#include <QLineEdit>

GLWidget::GLWidget(QWidget* parent) :
    QGLWidget(parent)
{
    //QLineEdit* positionXInput = parent->findChild<QLineEdit*>("positionXInput");
    //positionXInput->setText("2");
    /*
    std::string str = this->objectName();
    const QObjectList& children = parent->children();
    for(int i = 0; i < children.count(); i++){
        QObject* object = children.at(i);
        std::cout << object->objectName() << std::endl;
    }*/

    camera = new Camera();
    camera->move(0,0,0);
    perspectiveProjection = new PerspectiveProjection(0.5f);

    renderableObjects.push_back(new Torus(0.5, 0.12));
    renderableObjects.push_back(new Torus(0.5, 0.12));
    renderableObjects[1]->move(0, -1, -1);

    // Start main loop
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);

    this->setFocusPolicy(Qt::ClickFocus);
}

GLWidget::~GLWidget(){
    delete camera;
    delete perspectiveProjection;

    for(unsigned int i = 0; i < renderableObjects.size(); i++){
        delete renderableObjects[i];
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event){
    // Move X
    if(event->key() == Qt::Key_Left){
        camera->move(-0.1, 0, 0);
    }if(event->key() == Qt::Key_Right){
        camera->move(0.1, 0, 0);
    }
    // Move Y
    if(event->key() == Qt::Key_Up){
        camera->move(0.0, 0.1, 0);
    }else if(event->key() == Qt::Key_Down){
        camera->move(0.0,-0.1, 0);
    }

    // Rotate
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

    // Scale
    if(event->key() == Qt::Key_Plus){
        camera->scaleDt(0.05);
    }else if(event->key() == Qt::Key_Minus){
        camera->scaleDt(-0.05);
    }

    if(event->key() == Qt::Key_Q){
        Torus* torus = (Torus*)renderableObjects[0];
        torus->updateNetSize(30,30);
    }
    if(event->key() == Qt::Key_W){
        Torus* torus = (Torus*)renderableObjects[0];
        torus->updateNetSize(90,90);
    }
    if(event->key() == Qt::Key_E){
        Torus* torus = (Torus*)renderableObjects[0];
        torus->updateNetSize(120,120);
    }
    if(event->key() == Qt::Key_R){
        Torus* torus = (Torus*)renderableObjects[0];
        torus->updateNetSize(180,180);
    }
    if(event->key() == Qt::Key_T){
        Torus* torus = (Torus*)renderableObjects[0];
        torus->updateNetSize(360,360);
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event){

}

void GLWidget::initializeGL() {
    glClearColor(0.2, 0.2, 0.2, 1);
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    const glm::mat4& viewMatrix = camera->getViewMatrix();
    const glm::mat4& projectionMatrix
            = perspectiveProjection->getProjectionMatrix();
    glm::mat4 VP = projectionMatrix * viewMatrix;

    for(unsigned int i = 0; i < renderableObjects.size(); i++){
        renderableObjects[i]->render(VP);
    }
}

void GLWidget::resizeGL(int width, int height){
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);
}
