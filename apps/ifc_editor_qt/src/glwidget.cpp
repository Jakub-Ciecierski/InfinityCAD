#include "editor_window.h"
#include "glwidget.h"
#include <iostream>


#include <QColorDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDesktopWidget>
#include <infinity_cad/rendering/projections/stereoscopic_projection.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/rendering/cameras/camera_fps.h>
#include <infinity_cad/rendering/color/color_convertor.h>
#include "infinity_cad/settings/settings.h"
#include <infinity_cad/math/math.h>
#include <infinity_cad/geometry/quaternion.h>
#include <widgets/scene_list/scene_tree.h>
#include <system/object_manager.h>

#include "ui_mainwindow.h"

using namespace glm;

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
const int KEY_Z = 10;
const int KEYS_COUNT = 11;

GLWidget::GLWidget(QWidget* parent) :
    QGLWidget(parent)
{
    setupRenderer();
    setupFocusPolicy();

    startMainLoop();

    isLeftMousePressed = false;
    isLeftMouseDrag = false;
    isRightMouseDrag = false;
    isMiddleMouseDrag = false;

    setMouseTracking(true);

    //setContextMenuPolicy(Qt::CustomContextMenu);
}

GLWidget::~GLWidget(){
    delete renderer;
    delete ray;
    delete mouseTracker;
}

void GLWidget::setupRenderer(){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    scene = objectFactory.createScene("Scene");
    renderer = new Renderer(scene);
    renderer->resize(500,500);

    Projection* stereoscopicProjection = new StereoscopicProjection(
                renderer->getWindowWidthPointer(),
                renderer->getWindowHeightPointer(),
                1.0f, 0.06);

    CameraFPS* fpsCamera = (CameraFPS*)
            objectFactory.createCameraFPS("FPS Camera", stereoscopicProjection);

    scene->addCamera(fpsCamera);
    scene->setActiveCamera(fpsCamera);

    ray = new RayCast(fpsCamera);
    mouseTracker = new MouseTracker(ray, renderer);

    setCameraDefaultPosition();

    selectionBox = NULL;
}

void GLWidget::setupFocusPolicy(){
    this->setFocusPolicy(Qt::ClickFocus);
}

void GLWidget::setCameraDefaultPosition(){
    CameraFPS* camera = (CameraFPS*)renderer->getScene()->getActiveCamera();
    camera->moveTo(1.85, 1.5, -1.93);
    camera->rotateTo(-225, -151, 0);

    scene->rotateTo(0,0,0);

    Cross* cross = renderer->getScene()->getCross();
    cross->moveTo(0,0,0);
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

//------------------------------//
//  KEY EVENTS
//------------------------------//

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
    if(event->key() == Qt::Key_Z){
        keys[KEY_Z] = true;
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
    if(event->key() == Qt::Key_Z){
        keys[KEY_Z] = false;
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
    if(event->buttons() & Qt::LeftButton &&
            !(event->buttons() & Qt::RightButton) &&
            !(event->button() & Qt::MiddleButton)){
        Cross* cross = renderer->getScene()->getCross();
        mouseTracker->update(event->x(), event->y());
        cross->pickCones(*ray,
                         renderer->getWindowWidth(),
                         renderer->getWindowHeight());

        if(cross->getPickedCone() == AxisType::NONE){
            ObjectFactory& objectFactory = ObjectFactory::getInstance();
            float x = renderer->xPixelToGLCoord(event->x());
            float y = renderer->yPixelToGLCoord(event->y());
            this->selectionBox = objectFactory.createSelectionBox("Box", vec2(x,y));
            this->selectionBox->updateEndPosition(vec2(x,y));
            scene->addRenderObjectFront(selectionBox);
        }

        leftMousePressPosition = event->pos();
        isLeftMousePressed = true;
        if(!isLeftMouseDrag){
            globalMouseDragPosition = QCursor::pos();
            mouseDragPosition = event->pos();
            isLeftMouseDrag = true;
        }
    }
    else if(event->buttons() & Qt::RightButton){
        rightMousePressPosition = event->pos();
        isRightMousePressed = true;
        if(!isRightMouseDrag){
            rightMouseDragPosition = event->pos();
            isRightMouseDrag = true;
        }

    }else if(event->button() & Qt::MiddleButton){
        middleMousePressPosition = event->pos();
        isMiddleMousePressed = true;
        if(!isMiddleMouseDrag){
            middleMouseDragPosition = event->pos();
            isMiddleMouseDrag = true;
        }
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    bool isLeftMouseStill = false;
    bool isRightMouseStill = false;
    int stillnessTolerance = 5;

    Cross* cross = renderer->getScene()->getCross();

    if(isLeftMousePressed) {
        if(selectionBox != NULL){
            SceneTree* sceneTree = EditorWindow::getInstance().getUI()->sceneTree;
            std::vector<RenderObject*> scannedObjects =
                    selectionBox->scanBox(cross, ray,
                                          renderer->getWindowWidth(),
                                          renderer->getWindowHeight());
            if(!(event->modifiers() & Qt::ControlModifier ||
                    event->modifiers() & Qt::ShiftModifier)){
                sceneTree->deactivateAll();
            }
            for(unsigned int i = 0; i < scannedObjects.size(); i++){
                sceneTree->activateObject(scannedObjects[i]);

                if(i == 0)
                    cross->moveTo(scannedObjects[i]);
            }
        }

        int dx = event->x() - leftMousePressPosition.x();
        int dy = event->y() - leftMousePressPosition.y();
        int dist = (dx*dx) + (dy*dy);

        if(dist <= stillnessTolerance )
            isLeftMouseStill = true;

        isLeftMousePressed = false;
    }
    if(isRightMousePressed){
        int dx = event->x() - rightMousePressPosition.x();
        int dy = event->y() - rightMousePressPosition.y();
        int dist = (dx*dx) + (dy*dy);

        if(dist <= stillnessTolerance )
            isRightMouseStill = true;

        isRightMousePressed = false;
    }
    if(isMiddleMousePressed){
        isMiddleMousePressed = false;
    }

    if(isLeftMouseStill){
        mouseTracker->update(event->x(), event->y());
        RenderObject * closestBody = cross->getClosestObject(*ray,
                                                             renderer->getWindowWidth(),
                                                             renderer->getWindowHeight());
        SceneTree* sceneTree = EditorWindow::getInstance().getUI()->sceneTree;

        if(closestBody == NULL) {
            sceneTree->deactivateAll();
        } else{
            if(event->modifiers() & Qt::ControlModifier ||
                    event->modifiers() & Qt::ShiftModifier){
                sceneTree->activateObject(closestBody);
            }else{
                sceneTree->deactivateAll();
                sceneTree->activateObject(closestBody);
                cross->moveTo(closestBody);
            }
        }
    }

    isLeftMouseDrag = false;
    isRightMouseDrag = false;
    isMiddleMouseDrag = false;

    cross->unPickCones();
    cross->deactivateGrab();

    scene->removeObject(selectionBox);
    selectionBox = NULL;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    CameraFPS* camera = (CameraFPS*)renderer->getScene()->getActiveCamera();
    Cross* cross = renderer->getScene()->getCross();

    if(isLeftMouseDrag){
        AxisType axis = cross->getPickedCone();
        if(axis != NONE){

            float moveDist = 0.0005 * 1;
            int dx = event->x() - mouseDragPosition.x();
            int dy = event->y() - mouseDragPosition.y();

            if(event->modifiers() & Qt::ControlModifier){
                moveDist *= 3.0;
            }

            SceneTree* sceneTree = EditorWindow::getInstance().getUI()->sceneTree;
            cross->activateGrab(sceneTree->getSelectedObjects());

            const vec3& xDir = cross->getXDirection();
            const vec3& yDir = cross->getYDirection();
            const vec3& zDir = cross->getZDirection();
            const vec3& camDir = camera->getDirection();
            const vec3& camUp = camera->getUp();
            const vec3& camRight = camera->getRight();

            float xAngle = ifc::dot(xDir, camDir);
            //float yAngle = ifc::dot(yDir, camDir);
            float zAngle = ifc::dot(zDir, camDir);

            float zUpAngle = ifc::dot(zDir, camUp);
            float xUpAngle = ifc::dot(xDir, camUp);
            float yUpAngle = ifc::dot(yDir, camUp);

            float xRightAngle = ifc::dot(xDir, camRight);
            float zRightAngle = ifc::dot(zDir, camRight);

            int dXAxis = -dy;
            int dYAxis = -dy;
            int dZAxis = -dy;

            if(abs(zAngle) >= 0.0f && abs(zAngle) <= 0.5f &&
                    abs(zUpAngle) < 0.5f){
                dZAxis = dx;
                if(zRightAngle < 0.0f){
                    dZAxis = -dZAxis;
                }
            }else{
                if(zUpAngle < 0.0f){
                    dZAxis = -dZAxis;
                }
            }
            if(abs(xAngle) >= 0.0f && abs(xAngle) <= 0.5f &&
                    abs(xUpAngle) < 0.5f ){
                dXAxis = dx;
                if(xRightAngle < 0.0f){
                    dXAxis = -dXAxis;
                }
            }else{
                if(xUpAngle < 0.0f){
                    dXAxis = -dXAxis;
                }
            }
            if(yUpAngle < 0.0f){
                dYAxis = -dYAxis;
            }

            if(event->modifiers() & Qt::ShiftModifier){
                ObjectManager& objManager = ObjectManager::getInstance();
                float sens = (float)dYAxis * moveDist;
                objManager.colapsSelectedPoints_NoRemove(sens);
            }else{

                // Z
                if(axis == AxisType::ZAxis){
                    cross->move(0, 0, (float)dZAxis * moveDist);
                }
                // Y
                else if(axis == YAxis){
                    cross->move(0, (float)dYAxis * moveDist, 0);
                }
                // X
                else if(axis == XAxis){
                    cross->move((float)dXAxis * moveDist, 0, 0);
                }
            }
        }
        else{
            float x = renderer->xPixelToGLCoord(event->x());
            float y = renderer->yPixelToGLCoord(event->y());
            if(selectionBox != NULL)
                selectionBox->updateEndPosition(vec2(x,y));
        }

        mouseDragPosition = event->pos();
    }

    // Cross
    if(isRightMouseDrag){
        int dx = event->x() - rightMouseDragPosition.x();
        int dy = event->y() - rightMouseDragPosition.y();

        if(isFPSCamera){
            camera->rotate(dx * camera->mouseSpeed, dy * camera->mouseSpeed, 0.0f);
        }else{
            vec3 rotatedPos = Quaternion::rotate(camera->getPosition(),
                                                 vec3(0,1,0),
                                                 dx * camera->mouseSpeed);

            camera->moveTo(rotatedPos);
            camera->setDirection(normalize(
                                     -vec3(camera->getPosition().x,
                                           camera->getPosition().y,
                                           camera->getPosition().z)));
        }
        /*
        const vec3& xDir = cross->getXDirection();
        const vec3& yDir = cross->getYDirection();
        const vec3& zDir = cross->getZDirection();
        const vec3& camDir = camera->getDirection();
        const vec3& camUp = camera->getUp();
        const vec3& camRight = camera->getRight();

        float xAngle = ifc::dot(xDir, camDir);
        //float yAngle = ifc::dot(yDir, camDir);
        float zAngle = ifc::dot(zDir, camDir);

        float zUpAngle = ifc::dot(zDir, camUp);
        float xUpAngle = ifc::dot(xDir, camUp);
        float yUpAngle = ifc::dot(yDir, camUp);

        float xRightAngle = ifc::dot(xDir, camRight);
        float zRightAngle = ifc::dot(zDir, camRight);

        float moveDist = 0.002 * 5;
        int dx = event->x() - rightMouseDragPosition.x();
        int dy = event->y() - rightMouseDragPosition.y();

        int dXAxis = -dy;
        int dYAxis = -dy;
        int dZAxis = -dy;

        if(abs(zAngle) >= 0.0f && abs(zAngle) <= 0.5f &&
                abs(zUpAngle) < 0.5f){
            dZAxis = dx;
            if(zRightAngle < 0.0f){
                dZAxis = -dZAxis;
            }
        }else{
            if(zUpAngle < 0.0f){
                dZAxis = -dZAxis;
            }
        }
        if(abs(xAngle) >= 0.0f && abs(xAngle) <= 0.5f &&
                abs(xUpAngle) < 0.5f ){
            dXAxis = dx;
            if(xRightAngle < 0.0f){
                dXAxis = -dXAxis;
            }
        }else{
            if(xUpAngle < 0.0f){
                dXAxis = -dXAxis;
            }
        }
        if(yUpAngle < 0.0f){
            dYAxis = -dYAxis;
        }

        // Z
        if(event->modifiers() & Qt::ControlModifier){
            cross->move(0, 0, (float)dZAxis * moveDist);
        }
        // Y
        else if(event->modifiers() & Qt::ShiftModifier){
            cross->move(0, (float)dYAxis * moveDist, 0);
        }
        // X
        else{
            cross->move((float)dXAxis * moveDist, 0, 0);
        }
*/
        rightMouseDragPosition = event->pos();
    }
    if(isMiddleMouseDrag){
        float moveDist = 0.002 * 7;

        if(event->modifiers() & Qt::ControlModifier){
            moveDist *= 3;
        }

        int dx = event->x() - middleMouseDragPosition.x();
        int dy = event->y() - middleMouseDragPosition.y();

        camera->moveUp(-dy * moveDist);
        //camera->move(0, -dy * moveDist, 0);
        camera->moveRight(-dx * moveDist);

        middleMouseDragPosition = event->pos();
    }
}

void GLWidget::wheelEvent(QWheelEvent* event){
    CameraFPS* camera = (CameraFPS*)renderer->getScene()->getActiveCamera();

    float speedBoost = 0.01;
    if(event->modifiers() & Qt::ControlModifier){
        speedBoost = 0.05;
    }
    int x = event->delta();
    camera->moveForward(x * speedBoost);
}

void GLWidget::focusOutEvent(QFocusEvent* event){
    for(int i = 0; i < KEYS_COUNT; i++){
        keys[i] = false;
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
        SceneTree* sceneTree = EditorWindow::getInstance().getUI()->sceneTree;
        cross->activateGrab(sceneTree->getSelectedObjects());
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

    updateCrossView();
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

void GLWidget::setCUDA(bool value){
    ifc::RUN_CUDA = value;
}

void GLWidget::setFPSCamera(bool value){
    isFPSCamera = value;
    /*
    if(!isFPSCamera){
        setCameraDefaultPosition();
    }*/
}

void GLWidget::leftEyeColorPicker(){
    StereoscopicProjection* projection
           = (StereoscopicProjection*)renderer->getScene()->getActiveCamera()->getProjection();

    Color gmInitColor = projection->getLeftColor();

    QColor qInitColor = GMColorToQColor(gmInitColor);

    QColorDialog *ColorDialog = new QColorDialog(this);
    QColor qcolor = ColorDialog->getColor(qInitColor, this, "Left Eye");
    if(qcolor.isValid()){
        Color gmColor = QColorToGMColor(qcolor);
        projection->setLeftColor(gmColor);
    }
    delete ColorDialog;

}

void GLWidget::rightEyeColorPicker(){
    StereoscopicProjection* projection
           = (StereoscopicProjection*)renderer->getScene()->getActiveCamera()->getProjection();

    Color gmInitColor = projection->getRightColor();
    QColor qInitColor = GMColorToQColor(gmInitColor);

    QColorDialog *ColorDialog = new QColorDialog(this);
    QColor qcolor = ColorDialog->getColor(qInitColor, this, "Right Eye");
    if(qcolor.isValid()){
        Color gmColor = QColorToGMColor(qcolor);
        projection->setRightColor(gmColor);
    }
    delete ColorDialog;
}

void GLWidget::backgroundColorPicker(){
    Color gmInitColor = scene->getColor();
    QColor qInitColor = GMColorToQColor(gmInitColor);
    //QColor qcolor = QColorDialog::getColor(qInitColor, this, "Background");

    QColorDialog *ColorDialog = new QColorDialog(this);
    QColor qcolor = ColorDialog->getColor(qInitColor, this,"Background");
    if(qcolor.isValid()){
        Color gmColor = QColorToGMColor(qcolor);
        scene->setColor(gmColor);
    }
    delete ColorDialog;
}

void GLWidget::homeButtonClicked(){
    setCameraDefaultPosition();
}

void GLWidget::crossActiveCheckBox(bool value){
    Cross* cross = renderer->getScene()->getCross();
    cross->setGrabActive(value);
}

void GLWidget::moveObject(const SceneID& id, glm::vec3& pos){
    RenderObject * body = scene->getRenderBody(id);

    body->moveTo(pos);
}

#include "moc_glwidget.cpp"

