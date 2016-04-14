#include <system/object_manager.h>

#include "editor_window.h"
#include "ui_mainwindow.h"
#include "system/ifc_types.h"

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

    bSplineBinding = new BSplineBinding(scene, sceneTree);
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
        addChildItem(selectedBezierItems[i], pointItem);
    }

    selectedBezierItems = sceneTree->getSelectedItems(RB_BSPLINE_TYPE);
    for(unsigned int i = 0; i < selectedBezierItems.size(); i++){
        addChildItem(selectedBezierItems[i], pointItem);
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
        addChildItem(bezierItem, selectedPointItems[i]);
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

ObjectManager::~ObjectManager(){
    delete bSplineBinding;
}

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
    }else if(type == RB_BSPLINE_TYPE){
        bSplineBinding->createBSpline(name);
    }

    if(body != NULL){
        body->moveTo(scene->getCross());
    }
}

void ObjectManager::addChildItem(Item* parentItem,
                                 Item* childItem){
    if(parentItem == NULL || childItem == NULL ||
            childItem->type != RB_POINT_TYPE){
        return;
    }

    // Check if point is already added
    for(unsigned int i = 0; i < parentItem->children.size();i++){
        Item* currentChildItem = parentItem->children[i];
        if(currentChildItem != NULL && *currentChildItem == *childItem)
            return;
    }

    if(parentItem->type == RB_BEZIER_TYPE ||
            parentItem->type == RB_BSPLINE_TYPE ){
        Spline* spline = static_cast<Spline*>(parentItem->object);
        Point* point = static_cast<Point*>(childItem->object);

        sceneTree->addChildItem(parentItem, childItem);
        spline->addPoint(point);
    }


    return;
}

void ObjectManager::removeChildItem(Item* objectItem){
    Item* bezierItem = objectItem->parent;
    if(bezierItem == NULL || objectItem == NULL ||
            objectItem->type != RB_POINT_CLONE_TYPE){
        return;
    }

    if(bezierItem->type == RB_BEZIER_TYPE ||
            bezierItem->type == RB_BSPLINE_TYPE ){
        Spline* spline = static_cast<Spline*>(bezierItem->object);
        Point* point = static_cast<Point*>(objectItem->object);

        sceneTree->deleteObject(objectItem);
        spline->removePoint(point);
    }
}

void ObjectManager::moveUpItem(Item* objectItem){
    Item* bezierItem = objectItem->parent;
    if(bezierItem == NULL || objectItem == NULL ||
            objectItem->type != RB_POINT_CLONE_TYPE) {
        return;
    }

    if(bezierItem->type == RB_BEZIER_TYPE ||
            bezierItem->type == RB_BSPLINE_TYPE ){
        Spline* spline = static_cast<Spline*>(bezierItem->object);
        Point* point = static_cast<Point*>(objectItem->object);

        sceneTree->moveItemUpWithinParent(objectItem);
        spline->moveUp(point);
    }
}

void ObjectManager::moveDownItem(Item* objectItem){
    Item* bezierItem = objectItem->parent;
    if(bezierItem == NULL || objectItem == NULL ||
            objectItem->type != RB_POINT_CLONE_TYPE){
        return;
    }
    if(bezierItem->type == RB_BEZIER_TYPE ||
            bezierItem->type == RB_BSPLINE_TYPE ){
        Spline* spline = static_cast<Spline*>(bezierItem->object);
        Point* point = static_cast<Point*>(objectItem->object);

        sceneTree->moveItemDownWithinParent(objectItem);
        spline->moveDown(point);
    }
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

//--------------------------//
//  DEBUG
//--------------------------//

void ObjectManager::TEST_BSPLINE(){
    addObject(RB_POINT_TYPE, "test1");
    addObject(RB_POINT_TYPE, "test2");
    addObject(RB_POINT_TYPE, "test3");
    addObject(RB_POINT_TYPE, "test4");

    addObject(RB_BSPLINE_TYPE, "test5");

    const std::vector<Item*>& allItems = sceneTree->getAllItems();
    Item* spline;
    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if(item->type == RB_BSPLINE_TYPE)
            spline = item;
    }

    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if(item->type == RB_POINT_TYPE){
            item->object->moveTo((float)i/5, (float)i/5 , 0);
            addChildItem(spline, item);
        }

    }

}