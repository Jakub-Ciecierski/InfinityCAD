#include "object_manager.h"
#include "editor_window.h"
#include "ui_mainwindow.h"
#include "widgets/objects_list/objects_settings.h"

#include <gm/cameras/camera.h>
#include <gm/scene/scene_id_factory.h>
#include <gm/scene/scene_id.h>
#include <gm/rendering/render_bodies/torus.h>
#include <gm/rendering/render_bodies/point.h>
#include <gm/color/color_settings.h>


using namespace std;
using namespace Ui;

//--------------------------//
//  PRIVATE
//--------------------------//

ObjectManager::ObjectManager(){
    EditorWindow& m = EditorWindow::getInstance();
    MainWindow* ui = m.getUI();

    this->glWidget = ui->glRendererWidget;
    this->scene = ui->glRendererWidget->getRenderer()->getScene();
    this->sceneTree = ui->sceneTree;
}

void ObjectManager::addTorus(string name){
    Torus* t = new Torus();

    SceneID id = this->scene->addRenderObject(t);
    sceneTree->addObject(name, id);
}

void ObjectManager::addPoint(string name){
    Point* p = new Point();

    SceneID id = this->scene->addRenderObject(p);
    sceneTree->addObject(name, id);
}

string ObjectManager::getDefaultName(string type, SceneID id){
    string defaultName = type + "_" + to_string(id.getKey());

    return defaultName;
}

//--------------------------//
//  PUBLIC
//--------------------------//

ObjectManager& ObjectManager::getInstance(){
    static ObjectManager objectManager;

    return objectManager;
}

void ObjectManager::addObject(string type){
    SceneID nextID = this->scene->getNextAvailableID();
    string defaultName = getDefaultName(type, nextID);

    addObject(type, defaultName);
}

void ObjectManager::addObject(string type, string name){
    if(sceneTree->getItemIndex(name) >= 0) {
        string title = "Name";
        string text = "Name: " + name + " already exists. Try other name";
        EditorWindow::getInstance().showInfoBox(title, text);
        return;
    }

    if(type == RB_TORUS_NAME){
        addTorus(name);
    }else if(type == RB_POINT_NAME){
        addPoint(name);
    }
}

void ObjectManager::deleteObject(string name){
    string text = "Delete " + name + " ?";
    string title = "Delete";
    if(!EditorWindow::getInstance().showQuestionBox(title, text)) return;

    SceneID id = this->sceneTree->deleteObject(name);
    this->scene->removeObject(id);
}

void ObjectManager::changeName(string srcName){
    string text = "Input new name";
    string title = "Change Name";
    string dstName = EditorWindow::getInstance().showInputBox(title, text);

    if(sceneTree->getItemIndex(dstName) >= 0) {
        string title = "Name";
        string text = "Name: " + dstName + " already exists. Try other name";
        EditorWindow::getInstance().showInfoBox(title, text);
        return;
    }
    if(dstName.empty()) return;
    sceneTree->changeName(srcName, dstName);
}

void ObjectManager::moveCross(string srcName){
    SceneID id = sceneTree->getID(srcName);

    RenderBody* body = scene->getRenderBody(id);
    Cross* cross = scene->getCross();
    cross->moveTo(body);

    EditorWindow& m = EditorWindow::getInstance();
    MainWindow* ui = m.getUI();
    ui->glRendererWidget->updateCrossView();
    /*
    const glm::vec3& pos = cross->getPosition();


    ui->positionXInput->setText(QString::number(pos.x, 'f', 2));
    ui->positionYInput->setText(QString::number(pos.y, 'f', 2));
    ui->positionZInput->setText(QString::number(pos.z, 'f', 2));

    int pX = ui->glRendererWidget->getRenderer()->
            xGLToPixelCoord(cross->transformed.x);
    int pY = ui->glRendererWidget->getRenderer()->
            yGLToPixelCoord(cross->transformed.y);


    ui->angleXInput->setText(QString::number(pX));
    ui->angleYInput->setText(QString::number(pY));*/
}

void ObjectManager::moveCamera(string objectName){
    SceneID id = sceneTree->getID(objectName);

    RenderBody* body = scene->getRenderBody(id);
    Camera* camera = scene->getActiveCamera();
    camera->moveTo(body);
}

void ObjectManager::setActive(SceneID id){

    RenderBody* body = scene->getRenderBody(id);
    body->setColor(COLOR_OBJECT_ACTIVE);

    /*
    EditorWindow& m = EditorWindow::getInstance();
    Ui::MainWindow* ui = m.getUI();

    QLineEdit* xEdit = ui->positionXInput;
    QLineEdit* yEdit = ui->positionYInput;
    QLineEdit* zEdit = ui->positionZInput;

    float x = xEdit->text().toFloat();
    float y = yEdit->text().toFloat();
    float z = zEdit->text().toFloat();

    glm::vec3 pos(x,y,z);
    glWidget->moveObject(id, pos);

    m.connect(xEdit, SIGNAL(textEdited(QString)),
            glWidget, SLOT(moveObject(SceneID,glm::vec3&)));
            */
}

void ObjectManager::setDeactive(SceneID id){
    RenderBody* body = scene->getRenderBody(id);
    body->setColor(COLOR_OBJECT_DEFAULT);
}
