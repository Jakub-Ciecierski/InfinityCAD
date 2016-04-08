#include <widgets/scene_list/context_menus/scene_cmenu_points.h>
#include "system/object_manager.h"

using namespace std;

SceneCMenuPoints::SceneCMenuPoints()
{
    init();
}

void SceneCMenuPoints::init(){
    initHandlers();

    addToBezierSubMenu = new SceneContextMenu();
    addToBezierSubMenu->setTitle(QString::fromStdString((SCENE_MENU_BEZIER_NAME)));


    this->addMenu(addToBezierSubMenu);
    this->addSeparator();

    this->addActionAndHandler((SCENE_MENU_DELETE_NAME),
                    SCENE_MENU_DELETE_HANDLER);
}

void SceneCMenuPoints::initHandlers(){
    SCENE_MENU_DELETE_NAME = "Delete";
    SCENE_MENU_DELETE_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.deleteObject(objectName);
    });

    SCENE_MENU_BEZIER_NAME = "Add to Bezier";
}

void SceneCMenuPoints::setBezierRoot(Item* bezierRoot){
    this->bezierRoot = bezierRoot;
}

QAction* SceneCMenuPoints::show(const QPoint& pos) {
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
