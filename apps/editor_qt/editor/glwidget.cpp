#include "mainwindow.h"
#include "glwidget.h"
#include <iostream>

#include <gm/objects/torus.h>
#include <gm/projections/perspective_projection.h>
#include <gm/cameras/camera.h>

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

    scene = new Scene();

    Projection* perspectiveProjection = new PerspectiveProjection(10.0f);
    Camera* camera = new Camera(perspectiveProjection);

    scene->addCamera(camera);
    scene->setActiveCamera(camera);

    scene->addRenderObject(new Torus(0.2, 0.1));

    // Start main loop
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);

    this->setFocusPolicy(Qt::ClickFocus);

    renderer = new Renderer(scene);

}

GLWidget::~GLWidget(){
    delete renderer;
}

void GLWidget::keyPressEvent(QKeyEvent *event){
    Camera* camera = renderer->getScene()->getActiveCamera();
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

}

void GLWidget::keyReleaseEvent(QKeyEvent *event){

}

void GLWidget::initializeGL() {
    renderer->initialize();
}

void GLWidget::paintGL(){
    renderer->render();
}

void GLWidget::resizeGL(int width, int height){
    renderer->resize(width, height);
}
