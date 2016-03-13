#include "mainwindow.h"
#include "glwidget.h"
#include <iostream>

#include <gm/color/color.h>
#include <gm/color/color_convertor.h>
#include <gm/objects/torus.h>
#include <gm/objects/cube.h>
#include <gm/projections/perspective_projection.h>
#include <gm/projections/stereoscopic_projection.h>
#include <gm/projections/indentity_projection.h>
#include <gm/cameras/camera_std.h>
#include <gm/cameras/camera_fps.h>

#include <QColorDialog>
#include <QMessageBox>
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

    startMainLoop();

    isMouseDrag = false;
}

GLWidget::~GLWidget(){
    delete renderer;
}

void GLWidget::setupRenderer(){
    scene = new Scene();

    Projection* perspectiveProjection = new PerspectiveProjection(1.0f);

    Projection* stereoscopicProjection = new StereoscopicProjection(1.0f, 0.06);
    CameraFPS* fpsCamera = new CameraFPS(stereoscopicProjection);
    fpsCamera->move(0,0,0);
    scene->addCamera(fpsCamera);
    scene->setActiveCamera(fpsCamera);

    Torus* t = new Torus(0.4, 0.3, 100, 100);
    Torus* tt = new Torus(0.2, 0.1);
    Torus* tt1 = new Torus(0.2, 0.1);
    Torus* tt2 = new Torus(0.2, 0.1);
    Torus* tt3 = new Torus(0.2, 0.1);
    t->move(0,0,5);
    tt->move(5,0,5);
    tt1->move(4,5,5);
    tt2->move(0,5,0);
    tt3->move(5,0,0);
    scene->addRenderObject(t);
    scene->addRenderObject(tt);
    scene->addRenderObject(tt1);
    scene->addRenderObject(tt2);
    scene->addRenderObject(tt3);


    renderer = new Renderer(scene);
}

void GLWidget::setupFocusPolicy(){
    this->setFocusPolicy(Qt::ClickFocus);
}

void GLWidget::startMainLoop(){
    // Start main loop
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);
}

Color GLWidget::QColorToGMColor(const QColor& qcolor){
    auto colorSTDToGL = [](int x){return (x / (float)COLOR_STD_MAX * COLOR_GL_MAX);};

    unsigned int R = qcolor.red();
    unsigned int G = qcolor.green();
    unsigned int B = qcolor.blue();

    float gmR = colorSTDToGL(R);
    float gmG = colorSTDToGL(G);
    float gmB = colorSTDToGL(B);

    Color gmColor = Color(gmR, gmG, gmB);

    return gmColor;
}

QColor GLWidget::GMColorToQColor(const Color& c){
    auto colorGLToSTD = [](float x){return COLOR_STD_MAX * x;};

    int R = colorGLToSTD(c.R);
    int G = colorGLToSTD(c.G);
    int B = colorGLToSTD(c.B);
    QColor qColor = QColor(R, G, B);

    return qColor;
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
    do_movement();
    renderer->update();
    renderer->render();
}

void GLWidget::resizeGL(int width, int height){
    renderer->resize(width, height);
}

//-----------------------------------------------------------//
//  SLOTS
//-----------------------------------------------------------//

void GLWidget::set3DRendering(bool value){
    renderer->getScene()->set3DRendering(value);
}

void GLWidget::set3DDistance(QString distance){
    StereoscopicProjection* projection
           = (StereoscopicProjection*)renderer->getScene()->getActiveCamera()->getProjection();

    bool isNumber;
    float distanceFloat = distance.toFloat(&isNumber);
    if(distance.isEmpty()) return;
    if(!isNumber){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Please input proper float number!");
        messageBox.setFixedSize(500,200);
        messageBox.show();
    }else{
        projection->setDistance3D(distanceFloat);
        projection->update();
    }
}

void GLWidget::leftEyeColorPicker(){
    StereoscopicProjection* projection
           = (StereoscopicProjection*)renderer->getScene()->getActiveCamera()->getProjection();

    Color gmInitColor = projection->getLeftColor();

    QColor qInitColor = GMColorToQColor(gmInitColor);
    QColor qcolor = QColorDialog::getColor(qInitColor, this);

    Color gmColor = QColorToGMColor(qcolor);

    projection->setLeftColor(gmColor);
}

void GLWidget::rightEyeColorPicker(){
    StereoscopicProjection* projection
           = (StereoscopicProjection*)renderer->getScene()->getActiveCamera()->getProjection();

    Color gmInitColor = projection->getRightColor();
    QColor qInitColor = GMColorToQColor(gmInitColor);
    QColor qcolor = QColorDialog::getColor(qInitColor, this);

    Color gmColor = QColorToGMColor(qcolor);

    projection->setRightColor(gmColor);
}

#include "moc_glwidget.cpp"
