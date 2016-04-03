#include "object_manager.h"
#include "editor_window.h"
#include "ui_mainwindow.h"
#include "system/ic_names.h"

#include <gm/cameras/camera.h>
#include <gm/scene/scene_id_factory.h>
#include <gm/scene/scene_id.h>
#include <gm/rendering/render_bodies/primitivies/torus.h>
#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/color/color_settings.h>
#include <gm/scene/object_factory.h>

using namespace std;
using namespace Ui;
using namespace ic;

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
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* t = objectFactory.createTorus(name);

    this->scene->addRenderObject(t);
    sceneTree->addObject(t, RB_TORUS_NAME);
}

void ObjectManager::addPoint(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* p = objectFactory.createPoint(name);

    this->scene->addRenderObject(p);
    sceneTree->addObject(p, RB_POINT_NAME);
}

void ObjectManager::addBezierCurve(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* p = objectFactory.createPoint(name);

    this->scene->addRenderObject(p);
    sceneTree->addObject(p, RB_BEZIER_NAME);
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
    if(sceneTree->objectExists(name)) {
        string title = "Name";
        string text = "Name: " + name + " already exists. Try other name";
        EditorWindow::getInstance().showInfoBox(title, text);
        return;
    }

    if(type == RB_TORUS_NAME){
        addTorus(name);
    }else if(type == RB_POINT_NAME){
        addPoint(name);
    }else if(type == RB_BEZIER_NAME){
        addBezierCurve(name);
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

    if(sceneTree->objectExists(dstName)) {
        string title = "Name";
        string text = "Name: " + dstName + " already exists. Try other name";
        EditorWindow::getInstance().showInfoBox(title, text);
        return;
    }
    if(dstName.empty()) return;

    sceneTree->changeName(srcName, dstName);
}

void ObjectManager::moveCross(string srcName){
    ItemTMP* item = sceneTree->getItemByName(srcName);
    if(item == NULL) return;
    Cross* cross = scene->getCross();
    cross->moveTo(item->object);

    EditorWindow& m = EditorWindow::getInstance();
    MainWindow* ui = m.getUI();
    ui->glRendererWidget->updateCrossView();

}

void ObjectManager::moveCamera(string objectName){
    ItemTMP* item = sceneTree->getItemByName(objectName);
    if(item == NULL) return;

    Camera* camera = scene->getActiveCamera();
    camera->moveTo(item->object);
}

void ObjectManager::setActive(SceneID id){

    RenderBody* body = scene->getRenderBody(id);
    body->setColor(COLOR_OBJECT_ACTIVE);
}

void ObjectManager::setDeactive(SceneID id){
    RenderBody* body = scene->getRenderBody(id);
    body->setColor(COLOR_OBJECT_DEFAULT);
}
