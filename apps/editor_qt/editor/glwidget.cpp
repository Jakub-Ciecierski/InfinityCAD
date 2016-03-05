#include "mainwindow.h"
#include "glwidget.h"
#include <iostream>

#include <gm/objects/torus.h>
#include <gm/objects/cube.h>
#include <gm/projections/perspective_projection.h>
#include <gm/cameras/camera_std.h>
#include <gm/cameras/camera_fps.h>

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
    CameraFPS* fpsCamera = new CameraFPS(perspectiveProjection);

    scene->addCamera(fpsCamera);
    scene->setActiveCamera(fpsCamera);

    Torus* t = new Torus(0.4, 0.3);
    t->move(0,0,5);
    Torus* tt = new Torus(0.2, 0.1);
    Cube* c  = new Cube();
    scene->addRenderObject(tt);
    scene->addRenderObject(t);
    scene->addRenderObject(c);

    // Start main loop
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);

    this->setFocusPolicy(Qt::ClickFocus);

    renderer = new Renderer(scene);

    isMouseDrag = false;

}

GLWidget::~GLWidget(){
    delete renderer;
}

void GLWidget::keyPressEvent(QKeyEvent *event){
    float speedBoost = 1.0f;
    if(event->modifiers() & Qt::ShiftModifier){
        speedBoost = 10.0f;
    }
    CameraFPS* camera = (CameraFPS*)renderer->getScene()->getActiveCamera();

    if(event->key() == Qt::Key_W){
        camera->moveForward(speedBoost);
    }if(event->key() == Qt::Key_S){
        camera->moveBackward(speedBoost);
    }
    if(event->key() == Qt::Key_A){
        camera->moveLeft(speedBoost);
    }if(event->key() == Qt::Key_D){
        camera->moveRight(speedBoost);
    }
    if(event->key() == Qt::Key_Q){
        camera->moveDown(speedBoost);
    }
    if(event->key() == Qt::Key_E){
        camera->moveUp(speedBoost);
    }

    // Scale
    if(event->key() == Qt::Key_Plus){
        camera->scaleDt(0.05);
    }else if(event->key() == Qt::Key_Minus){
        camera->scaleDt(-0.05);
    }

}

void GLWidget::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        mouseDragPosition = event->pos();
        isMouseDrag = true;
    }
    else if(event->buttons() & Qt::RightButton){
        rightMouseDragPosition = event->pos();
        isRightMouseDrag = true;
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    isMouseDrag = false;
    isRightMouseDrag = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    float moveDist = 0.002 * 5;

    // Move in FPS style
    if(isMouseDrag){
        CameraFPS* camera = (CameraFPS*)renderer->getScene()->getActiveCamera();
        int dx = event->x() - mouseDragPosition.x();
        int dy = event->y() - mouseDragPosition.y();

        if(event->modifiers() & Qt::ShiftModifier){
            float speedBoost = 0.2;
            if(dx < 0)
                camera->moveLeft(speedBoost);
            if(dx > 0)
                camera->moveRight(speedBoost);
            if(dy < 0)
                camera->moveForward(speedBoost);
            if(dy > 0)
                camera->moveBackward(speedBoost);
        }else{
            camera->horizontalAngleDegree += dx * camera->mouseSpeed;
            camera->verticalAngleDegree -= dy * camera->mouseSpeed;
        }
        mouseDragPosition = event->pos();
    }
    // Move along axis
    if(isRightMouseDrag){
        Camera* camera = renderer->getScene()->getActiveCamera();

        int dx = event->x() - rightMouseDragPosition.x();
        int dy = event->y() - rightMouseDragPosition.y();

        // Z
        if(event->modifiers() & Qt::ControlModifier){
            camera->move(0, 0, (float)dy * moveDist);
        }
        // Y
        if(event->modifiers() & Qt::ShiftModifier){
            camera->move(0, (float)dy * moveDist, 0);
        }
        // X
        else{
            camera->move((float)dy * moveDist, 0, 0);
        }

        rightMouseDragPosition = event->pos();
    }
}

void GLWidget::wheelEvent(QWheelEvent* event){
    float speedBoost = 0.0001;
    if(event->modifiers() & Qt::ControlModifier){
        speedBoost = 0.001;
    }
    int x = event->delta();

    renderer->getScene()->scaleDt(x*speedBoost);
};

void GLWidget::initializeGL() {
    renderer->initialize();
}

void GLWidget::paintGL(){
    renderer->render();
}

void GLWidget::resizeGL(int width, int height){
    renderer->resize(width, height);
}
