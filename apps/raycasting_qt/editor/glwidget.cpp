#include "mainwindow.h"
#include "glwidget.h"
#include <iostream>

#include <gm/objects/torus.h>
#include <gm/objects/cube.h>
#include <gm/projections/perspective_projection.h>
#include <gm/projections/indentity_projection.h>
#include <gm/cameras/camera_std.h>
#include <gm/cameras/camera_fps.h>

#include <QLineEdit>

bool keys[1024];
const int KEY_W = 0;
const int KEY_S = 1;
const int KEY_A = 2;
const int KEY_D = 3;
const int KEY_Q = 4;
const int KEY_E = 5;
const int KEY_MINUS = 6;
const int KEY_PLUS = 7;
const int KEY_SHIFT = 8;
const int KEY_SPACE = 9;

GLWidget::GLWidget(QWidget* parent) :
    QGLWidget(parent)
{
    setupRenderer();
    setupFocusPolicy();
    setupRayTracing();

    startMainLoop();

    isMouseDrag = false;
}

GLWidget::~GLWidget(){
    delete renderer;
}

void GLWidget::setupRenderer(){
    scene = new Scene();

    //Projection* perspectiveProjection = new PerspectiveProjection(1.0f);
    Projection* indentityProjection = new IndentityProjection();
    CameraFPS* fpsCamera = new CameraFPS(indentityProjection);
    fpsCamera->move(0,0,-5);
    scene->addCamera(fpsCamera);
    scene->setActiveCamera(fpsCamera);

    Torus* t = new Torus(0.4, 0.3);
    t->move(0,0,5);
    Torus* tt = new Torus(0.2, 0.1);
    Cube* c  = new Cube();
    scene->addRenderObject(tt);
    scene->addRenderObject(t);
    scene->addRenderObject(c);

    renderer = new Renderer(scene);
}

void GLWidget::setupFocusPolicy(){
    this->setFocusPolicy(Qt::ClickFocus);
}

void GLWidget::setupRayTracing(){
    sliderValueDivider = 10.0f;

    float a = 6.6;
    float b = 10.6;
    float c = 4.6;
    ellipsoid = new Ellipsoid(a, b, c);

    ellipsoidARadiusChanged(a * sliderValueDivider);
    emit ellipsoidBRadiusChanged(b * sliderValueDivider);
    emit ellipsoidCRadiusChanged(c * sliderValueDivider);

    emit ellipsoidBRadiusChanged(b * sliderValueDivider);
    emit ellipsoidCRadiusChanged(c * sliderValueDivider);

    ray = new Ray(renderer);
    ray->ellipsoid = ellipsoid;
    ray->intesityExponent = 2;

    continueAdaptiveRendering = true;
    sliderValueMax = 100;

    scene->scale(2.3);
}

void GLWidget::startMainLoop(){
    // Start main loop
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);
}

//-----------------------------------------------------------//
//  KEY EVENTS
//-----------------------------------------------------------//

void GLWidget::keyPressEvent(QKeyEvent *event){

    if(event->key() == Qt::Key_W){
        keys[KEY_W] = true;
    }if(event->key() == Qt::Key_S){
        keys[KEY_S] = true;
    }
    if(event->key() == Qt::Key_A){
        keys[KEY_A] = true;
    }if(event->key() == Qt::Key_D){
        keys[KEY_D] = true;
    }
    if(event->key() == Qt::Key_Q){
        keys[KEY_Q] = true;
    }
    if(event->key() == Qt::Key_E){
        keys[KEY_E] = true;
    }
    if(event->key() == Qt::Key_Plus){
        keys[KEY_PLUS] = true;
    }else if(event->key() == Qt::Key_Minus){
        keys[KEY_MINUS] = true;
    }
    if(event->key() == Qt::Key_Shift){
        keys[KEY_SHIFT] = true;
    }
    if(event->key() == Qt::Key_Space){
        keys[KEY_SPACE] = true;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_W){
        keys[KEY_W] = false;
    }if(event->key() == Qt::Key_S){
        keys[KEY_S] = false;
    }
    if(event->key() == Qt::Key_A){
        keys[KEY_A] = false;
    }if(event->key() == Qt::Key_D){
        keys[KEY_D] = false;
    }
    if(event->key() == Qt::Key_Q){
        keys[KEY_Q] = false;
    }
    if(event->key() == Qt::Key_E){
        keys[KEY_E] = false;
    }
    if(event->key() == Qt::Key_Plus){
        keys[KEY_PLUS] = false;
    }else if(event->key() == Qt::Key_Minus){
        keys[KEY_MINUS] = false;
    }
    if(event->key() == Qt::Key_Shift){
        keys[KEY_SHIFT] = false;
    }
    if(event->key() == Qt::Key_Space){
        keys[KEY_SPACE] = false;
    }
}

//-----------------------------------------------------------//
//  MOUSE
//-----------------------------------------------------------//

void GLWidget::mousePressEvent(QMouseEvent *event){
    continueAdaptiveRendering = true;

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
        continueAdaptiveRendering = true;
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
        continueAdaptiveRendering = true;
        Camera* camera = renderer->getScene()->getActiveCamera();

        //int dx = event->x() - rightMouseDragPosition.x();
        int dy = event->y() - rightMouseDragPosition.y();

        // Z
        if(event->modifiers() & Qt::ControlModifier){
            camera->move(0, 0, (float)dy * moveDist);
        }
        // Y
        else if(event->modifiers() & Qt::ShiftModifier){
            camera->move(0, (float)dy * moveDist, 0);
        }
        // X
        else{
            camera->move((float)dy * moveDist, 0, 0);
        }


        rightMouseDragPosition = event->pos();
    }
}

bool GLWidget::do_movement(){
    bool changed = false;

    CameraFPS* camera = (CameraFPS*)renderer->getScene()->getActiveCamera();

    float speedBoost = 0.6f;
    if(keys[KEY_SPACE]){
        speedBoost = 3.0f;
    }

    if(keys[KEY_W]){
        camera->moveForward(speedBoost);
        changed = true;
    }
    if(keys[KEY_S]){
        camera->moveBackward(speedBoost);
        changed = true;
    }
    if(keys[KEY_A]){
        camera->moveLeft(speedBoost);
        changed = true;
    }
    if(keys[KEY_D]){
        camera->moveRight(speedBoost);
        changed = true;
    }
    if(keys[KEY_Q]){
        camera->moveDown(speedBoost/3);
        changed = true;
    }
    if(keys[KEY_E]){
        camera->moveUp(speedBoost/3);
        changed = true;
    }
    if(keys[KEY_MINUS]){
        camera->scaleDt(0.05);
        changed = true;
    }
    if(keys[KEY_PLUS]){
        camera->scaleDt(-0.05);
        changed = true;
    }

    return changed;
}

void GLWidget::wheelEvent(QWheelEvent* event){
    continueAdaptiveRendering = true;
    float speedBoost = 0.0001;
    if(event->modifiers() & Qt::ControlModifier){
        speedBoost = 0.001;
    }
    int x = event->delta();

    renderer->getScene()->scaleDt(x*speedBoost);
};

//-----------------------------------------------------------//
//  OPENGL
//-----------------------------------------------------------//

void GLWidget::initializeGL() {
    renderer->initialize();
}

void GLWidget::paintGL(){

    bool doneMovement = do_movement();
    if(doneMovement){
        continueAdaptiveRendering = true;
    }
    renderer->update();

    if(doAdaptive){
        static int exponent = 2;
        if(continueAdaptiveRendering){
            if(ray->adaptiveRayCasting(*renderer,exponent)) {
                ray->resetAdaptiveRayCasting();
                continueAdaptiveRendering = false;
            }
        }
    }else{
        ray->rayCasting(*renderer);
    }

    //renderer->render();
}

void GLWidget::resizeGL(int width, int height){
    renderer->resize(width, height);
}

//-----------------------------------------------------------//
//  SLOTS
//-----------------------------------------------------------//

void GLWidget::setEllipsoidARadius(int  value){
    this->ellipsoid->setARadius(float(value) / sliderValueDivider);
    continueAdaptiveRendering = true;
}

void GLWidget::setEllipsoidBRadius(int  value){
    this->ellipsoid->setBRadius(float(value) / sliderValueDivider);
    continueAdaptiveRendering = true;
}

void GLWidget::setEllipsoidCRadius(int  value){
    this->ellipsoid->setCRadius(float(value) / sliderValueDivider);
    continueAdaptiveRendering = true;
}

void GLWidget::setRayLightIntensity(int  value){
    ray->intesityExponent = value;
    continueAdaptiveRendering = true;
}

void GLWidget::setDoAdaptive(bool v){
    doAdaptive = v;
}

#include "moc_glwidget.cpp"
