#include "editor_window.h"
#include "glwidget.h"
#include <iostream>

#include <gm/color/color.h>
#include <gm/color/color_convertor.h>
#include "gm/rendering/render_body.h"

#include <gm/rendering/render_bodies/torus.h>
#include <gm/rendering/render_bodies/cross.h>
#include <gm/rendering/render_bodies/cube.h>
#include <gm/projections/perspective_projection.h>
#include <gm/projections/stereoscopic_projection.h>
#include <gm/projections/indentity_projection.h>
#include <gm/cameras/camera_std.h>
#include <gm/cameras/camera_fps.h>

#include <QColorDialog>
#include <QMessageBox>
#include <QLineEdit>

#include "ui_mainwindow.h"

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
    isRightMouseDrag = false;
}

GLWidget::~GLWidget(){
    delete renderer;
    delete ray;
}

void GLWidget::setupRenderer(){
    scene = new Scene();

    Projection* stereoscopicProjection = new StereoscopicProjection(1.0f, 0.06);
    CameraFPS* fpsCamera = new CameraFPS(stereoscopicProjection);
    fpsCamera->move(0,0,0);

    scene->addCamera(fpsCamera);
    scene->setActiveCamera(fpsCamera);

    ray = new RayCast(fpsCamera);


/*
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
*/

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
        float xGL = renderer->xPixelToGLCoord(event->pos().x());
        float yGL = renderer->yPixelToGLCoord(event->pos().y());
        ray->update(xGL, yGL);

        Cross* cross = renderer->getScene()->getCross();
        cross->scanAndMoveToClosestObject(*ray, renderer->getWindowWidth(), renderer->getWindowHeight());

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
        Cross* cross = renderer->getScene()->getCross();

        //int dx = event->x() - rightMouseDragPosition.x();
        int dy = event->y() - rightMouseDragPosition.y();

        // Z
        if(event->modifiers() & Qt::ControlModifier){
            cross->move(0, 0, (float)dy * moveDist);
        }
        // Y
        else if(event->modifiers() & Qt::ShiftModifier){
            cross->move(0, (float)dy * moveDist, 0);
        }
        // X
        else{
            cross->move((float)dy * moveDist, 0, 0);
        }
        updateCrossView();

        rightMouseDragPosition = event->pos();
    }
}

void GLWidget::updateCrossView(){
    Cross* cross = renderer->getScene()->getCross();
    // -------------------------------------------
    const glm::vec3& pos = cross->getPosition();
    EditorWindow& m = EditorWindow::getInstance();
    Ui::MainWindow* ui = m.getUI();

    ui->positionXInput->setText(QString::number(pos.x, 'f', 2));
    ui->positionYInput->setText(QString::number(pos.y, 'f', 2));
    ui->positionZInput->setText(QString::number(pos.z, 'f', 2));

    const glm::vec3& bodyProjectedPosition = cross->getProjectedPosition();

    int pX = renderer->xGLToPixelCoord(bodyProjectedPosition.x);
    int pY = renderer->yGLToPixelCoord(bodyProjectedPosition.y);
    ui->angleXInput->setText(QString::number(pX));
    ui->angleYInput->setText(QString::number(pY));

    // -------------------------------------------
}

bool GLWidget::do_movement(){
    bool changed = false;

    CameraFPS* camera = (CameraFPS*)renderer->getScene()->getActiveCamera();
    Cross* cross = renderer->getScene()->getCross();

    float speedBoost = 0.3f;
    if(keys[KEY_SPACE]){
        speedBoost = 3.0f;
        cross->activateGrab();
    }else{
        cross->deactivateGrab();
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
        camera->moveDown(speedBoost);
        changed = true;
    }
    if(keys[KEY_E]){
        camera->moveUp(speedBoost);
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

Renderer* GLWidget::getRenderer(){
    return this->renderer;
}

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

    //ray->render(renderer->getScene()->getMVP());
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

void GLWidget::moveObject(const SceneID& id, glm::vec3& pos){
    RenderBody* body = scene->getRenderBody(id);

    body->moveTo(pos);
}

#include "moc_glwidget.cpp"
