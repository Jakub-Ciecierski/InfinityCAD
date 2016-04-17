#include "system/object_manager.h"
#include <iostream>
#include <widgets/scene_list/context_menus/scene_cmenu_point.h>
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

using namespace std;

SceneCMenuPoint::SceneCMenuPoint() {
    init();
}

void SceneCMenuPoint::init(){
    addToBezierSubMenu = new SceneContextMenu();
    addToBezierSubMenu->setTitle("Add to Bezier");

    this->addHandlerAndAction(SCENE_MENU_MOVE_CROSS_HANDLER);
    this->addHandlerAndAction(SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addMenu(addToBezierSubMenu);
    this->addSeparator();

    this->addHandlerAndAction(SCENE_MENU_DELETE_HANDLER);
}

void SceneCMenuPoint::setBezierRoot(Item* bezierRoot){
    this->bezierRoot = bezierRoot;
}

QAction* SceneCMenuPoint::show(const QPoint& pos) {
    addToBezierSubMenu->clear();

    if(bezierRoot->treeItem != NULL){
        for(unsigned int i = 0;i < bezierRoot->children.size();i++){
            Item* childBezierItem = bezierRoot->children[i];
            std::string childBezierName = childBezierItem->displayName;

            SceneCMHandler handler(
                        childBezierName,
                        [=](Item* item){
                    ObjectManager& objManager = ObjectManager::getInstance();
                    objManager.addChildItem(childBezierItem, item);
                });
            addToBezierSubMenu->addAction(childBezierName);
            this->addHandler(handler);
        }
    }

    return SceneContextMenu::show(pos);
}
