#include "object_manager.h"
#include "editor_window.h"
#include "ui_mainwindow.h"
#include "system/ic_names.h"

#include <gm/cameras/camera.h>
#include <gm/scene/scene_id_factory.h>
#include <gm/scene/scene_id.h>
#include <gm/rendering/render_bodies/primitivies/torus.h>
#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/rendering/render_bodies/curves/bezier_curve.h>

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

RenderBody* ObjectManager::addTorus(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* t = objectFactory.createTorus(name);

    this->scene->addRenderObject(t);
    sceneTree->addObject(t, RB_TORUS_TYPE);

    return t;
}

RenderBody* ObjectManager::addPoint(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* p = objectFactory.createPoint(name);

    this->scene->addRenderObject(p);
    Item* pointItem = sceneTree->addObject(p, RB_POINT_TYPE);

    // Add Point to all selected bezier curves
    std::vector<Item*> selectedBezierItems =
            sceneTree->getSelectedItems(RB_BEZIER_TYPE);
    for(unsigned int i = 0; i < selectedBezierItems.size(); i++){
        addPointToBezier(selectedBezierItems[i], pointItem);
    }

    return p;
}

RenderBody* ObjectManager::addBezierCurve(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderBody* p = objectFactory.createBezier(name);

    this->scene->addRenderObject(p);
    Item* bezierItem = sceneTree->addObject(p, RB_BEZIER_TYPE);

    // Add all selected points to curve
    std::vector<Item*> selectedPointItems =
            sceneTree->getSelectedItems(RB_POINT_TYPE);
    for(unsigned int i = 0; i < selectedPointItems.size(); i++){
        addPointToBezier(bezierItem, selectedPointItems[i]);
    }

    return p;
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

    RenderBody* body = NULL;
    if(type == RB_TORUS_TYPE){
        body = addTorus(name);
    }else if(type == RB_POINT_TYPE){
        body = addPoint(name);
    }else if(type == RB_BEZIER_TYPE){
        addBezierCurve(name);
    }

    if(body != NULL){
        body->moveTo(scene->getCross());
    }
}

void ObjectManager::addPointToBezier(Item* bezierItem,
                                     Item* objectItem){
    if(bezierItem == NULL || objectItem == NULL ||
            bezierItem->type != RB_BEZIER_TYPE ||
            objectItem->type != RB_POINT_TYPE){
        return;
        //throw new std::invalid_argument("Invalid Item types");
    }

    // Check if point is already added
    for(unsigned int i = 0; i < bezierItem->children.size();i++){
        Item* childItem = bezierItem->children[i];
        if(childItem != NULL && *childItem == *objectItem)
            return;
    }
    BezierCurve* bezierCurve = static_cast<BezierCurve*>(bezierItem->object);
    Point* point = static_cast<Point*>(objectItem->object);

    sceneTree->addPointToBezier(bezierItem, objectItem);
    bezierCurve->addPoint(point);

    return;
}

void ObjectManager::removePointFromBezier(Item* pointItem){
    Item* bezierItem = pointItem->parent;
    if(bezierItem == NULL || pointItem == NULL ||
            bezierItem->type != RB_BEZIER_TYPE ||
            pointItem->type != RB_POINT_BEZIER_TYPE){
        return;
        //throw new std::invalid_argument("Invalid Item types");
    }
    BezierCurve* bezierCurve = static_cast<BezierCurve*>(bezierItem->object);
    Point* point = static_cast<Point*>(pointItem->object);

    sceneTree->deleteObject(pointItem);
    bezierCurve->removePoint(point);
}

void ObjectManager::movePointUpBezier(Item* pointItem){
    Item* bezierItem = pointItem->parent;
    if(bezierItem == NULL || pointItem == NULL ||
            bezierItem->type != RB_BEZIER_TYPE ||
            pointItem->type != RB_POINT_BEZIER_TYPE) {
        return;
        //throw new std::invalid_argument("Invalid Item types");
    }

    BezierCurve* bezierCurve = static_cast<BezierCurve*>(bezierItem->object);
    Point* point = static_cast<Point*>(pointItem->object);

    sceneTree->moveItemUpWithinParent(pointItem);
    bezierCurve->moveUp(point);
}

void ObjectManager::movePointDownBezier(Item* pointItem){
    Item* bezierItem = pointItem->parent;
    if(bezierItem == NULL || pointItem == NULL ||
            bezierItem->type != RB_BEZIER_TYPE ||
            pointItem->type != RB_POINT_BEZIER_TYPE){
        return;
        //throw new std::invalid_argument("Invalid Item types");
    }
    BezierCurve* bezierCurve = static_cast<BezierCurve*>(bezierItem->object);
    Point* point = static_cast<Point*>(pointItem->object);

    sceneTree->moveItemDownWithinParent(pointItem);
    bezierCurve->moveDown(point);
}

void ObjectManager::deleteObject(Item* item){
    if(item->treeItem == NULL) return;

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
