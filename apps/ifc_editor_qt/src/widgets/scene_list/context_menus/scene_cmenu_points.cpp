#include <widgets/scene_list/context_menus/scene_cmenu_points.h>
#include "system/object_manager.h"
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

using namespace std;

SceneCMenuPoints::SceneCMenuPoints()
{
    init();
}

void SceneCMenuPoints::init(){
    addToBezierSubMenu = new SceneContextMenu();
    addToBezierSubMenu->setTitle("Add to Bezier");


    this->addMenu(addToBezierSubMenu);
    this->addSeparator();

    this->addHandlerAndAction(SCENE_MENU_DELETE_HANDLER);
}

void SceneCMenuPoints::setBezierRoot(Item* bezierRoot){
    this->bezierRoot = bezierRoot;
}

QAction* SceneCMenuPoints::show(const QPoint& pos) {
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
