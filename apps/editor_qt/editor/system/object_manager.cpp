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

#include <iostream>

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
    sceneTree->addObject(t, RB_TORUS_TYPE);
}

void ObjectManager::addPoint(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* p = objectFactory.createPoint(name);

    this->scene->addRenderObject(p);
    sceneTree->addObject(p, RB_POINT_TYPE);
}

void ObjectManager::addBezierCurve(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* p = objectFactory.createPoint(name);

    this->scene->addRenderObject(p);
    sceneTree->addObject(p, RB_BEZIER_TYPE);
}

string ObjectManager::getDefaultName(const Type& type){
    static int id_count = 0;
    string defaultName = type.type + "_" + to_string(id_count++);

    return defaultName;
}

//--------------------------//
//  PUBLIC
//--------------------------//

ObjectManager& ObjectManager::getInstance(){
    static ObjectManager objectManager;

    return objectManager;
}

void ObjectManager::addObject(const Type& type){
    string defaultName = getDefaultName(type);

    addObject(type, defaultName);
}

void ObjectManager::addObject(const Type& type, string name){
    if(sceneTree->objectExists(name)) {
        string title = "Name";
        string text = "Name: " + name + " already exists. Try other name";
        EditorWindow::getInstance().showInfoBox(title, text);
        return;
    }

    if(type == RB_TORUS_TYPE){
        addTorus(name);
    }else if(type == RB_POINT_TYPE){
        addPoint(name);
    }else if(type == RB_BEZIER_TYPE){
        addBezierCurve(name);
    }
}

void ObjectManager::addPointToBezier(Item* bezierName,
                                     Item* objectItem){
    sceneTree->addPointToBezier(bezierName, objectItem);
    // TODO Add Point to Bezier in Scene;
}

void ObjectManager::deleteObject(Item* item){
    string text = "Delete " + item->displayName + " ?";
    string title = "Delete";
    if(!EditorWindow::getInstance().showQuestionBox(title, text)) return;

    SceneID id = this->sceneTree->deleteObject(item);

    this->scene->removeObject(id);
}

void ObjectManager::changeName(Item* item){
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

    sceneTree->changeName(item, dstName);
}

void ObjectManager::moveCross(Item* item){
    if(item == NULL) return;
    Cross* cross = scene->getCross();
    cross->moveTo(item->object);

    EditorWindow& m = EditorWindow::getInstance();
    MainWindow* ui = m.getUI();
    ui->glRendererWidget->updateCrossView();

}

void ObjectManager::moveCamera(Item * item){
    if(item == NULL) return;

    Camera* camera = scene->getActiveCamera();
    camera->moveTo(item->object);
}

void ObjectManager::setActive(SceneID id){

    RenderBody* body = scene->getRenderBody(id);
    if(body != NULL)
        body->setColor(COLOR_OBJECT_ACTIVE);
}

void ObjectManager::setDeactive(SceneID id){
    RenderBody* body = scene->getRenderBody(id);
    if(body != NULL)
        body->setColor(COLOR_OBJECT_DEFAULT);
}
