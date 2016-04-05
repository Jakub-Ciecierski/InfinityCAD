#include "scene_cmenu_point.h"
#include "system/object_manager.h"
#include <iostream>

using namespace std;

SceneCMenuPoint::SceneCMenuPoint() {
    init();
}

void SceneCMenuPoint::init(){
    initHandlers();

    addToBezierSubMenu = new SceneContextMenu();
    addToBezierSubMenu->setTitle(QString::fromStdString((SCENE_MENU_BEZIER_NAME)));

    this->addActionAndHandler((SCENE_MENU_MOVE_CROSS_NAME),
                                   SCENE_MENU_MOVE_CROSS_HANDLER);
    this->addActionAndHandler((SCENE_MENU_MOVE_CAMERA_NAME),
                                   SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();
    this->addActionAndHandler((SCENE_MENU_CHANGE_NAME_NAME),
                    SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addMenu(addToBezierSubMenu);
    this->addSeparator();

    this->addActionAndHandler((SCENE_MENU_DELETE_NAME),
                    SCENE_MENU_DELETE_HANDLER);
}

void SceneCMenuPoint::initHandlers(){
    SCENE_MENU_MOVE_CROSS_NAME = "Move Cross";
    SCENE_MENU_MOVE_CROSS_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.moveCross(objectName);
    });

    SCENE_MENU_MOVE_CAMERA_NAME = "Move Camera";
    SCENE_MENU_MOVE_CAMERA_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.moveCamera(objectName);
    });
    SCENE_MENU_DELETE_NAME = "Delete";
    SCENE_MENU_DELETE_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.deleteObject(objectName);
    });

    SCENE_MENU_CHANGE_NAME_NAME = "Change Name";
    SCENE_MENU_CHANGE_NAME_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.changeName(objectName);
    });

    SCENE_MENU_BEZIER_NAME = "Add to Bezier";
}

void SceneCMenuPoint::setBezierRoot(Item* bezierRoot){
    this->bezierRoot = bezierRoot;
}

QAction* SceneCMenuPoint::show(const QPoint& pos) {
    addToBezierSubMenu->clear();

    if(bezierRoot->treeItem != NULL){
        for(int i = 0;i < bezierRoot->children.size();i++){
            Item* childBezierItem = bezierRoot->children[i];
            std::string childBezierName = childBezierItem->displayName;

            SceneCMHandler handler([=](Item* item){
                    ObjectManager objManager = ObjectManager::getInstance();
                    objManager.addPointToBezier(childBezierItem, item);
                });
            addToBezierSubMenu->addAction(childBezierName);
            this->addHandler(childBezierName, handler);
        }
    }

    return SceneContextMenu::show(pos);
}
