#include "scene_cmenu_point.h"
#include "system/object_manager.h"

using namespace std;

SceneCMenuPoint::SceneCMenuPoint() {
    init();
}

void SceneCMenuPoint::init(){
    initHandlers();

    addToBezierSubMenu = new ContextMenu();
    addToBezierSubMenu->setTitle(QString::fromStdString((SCENE_MENU_BEZIER_NAME)));

    this->addAction((SCENE_MENU_MOVE_CROSS_NAME),
                                   SCENE_MENU_MOVE_CROSS_HANDLER);
    this->addAction((SCENE_MENU_MOVE_CAMERA_NAME),
                                   SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();
    this->addAction((SCENE_MENU_CHANGE_NAME_NAME),
                    SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addMenu(addToBezierSubMenu);
    this->addSeparator();

    this->addAction((SCENE_MENU_DELETE_NAME),
                    SCENE_MENU_DELETE_HANDLER);
}

void SceneCMenuPoint::initHandlers(){
    SCENE_MENU_MOVE_CROSS_NAME = "Move Cross";
    SCENE_MENU_MOVE_CROSS_HANDLER = Handler([](string objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.moveCross(objectName);
    });

    SCENE_MENU_MOVE_CAMERA_NAME = "Move Camera";
    SCENE_MENU_MOVE_CAMERA_HANDLER = Handler([](string objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.moveCamera(objectName);
    });
    SCENE_MENU_DELETE_NAME = "Delete";
    SCENE_MENU_DELETE_HANDLER = Handler([](string objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.deleteObject(objectName);
    });

    SCENE_MENU_CHANGE_NAME_NAME = "Change Name";
    SCENE_MENU_CHANGE_NAME_HANDLER = Handler([](string objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.changeName(objectName);
    });

    SCENE_MENU_BEZIER_NAME = "Add to Bezier";
}

void SceneCMenuPoint::setBezierRoot(RootTreeItem* bezierRoot){
    this->bezierRoot = bezierRoot;
}

QAction* SceneCMenuPoint::show(const QPoint& pos) {
    QTreeWidgetItem* item = bezierRoot->item;

    addToBezierSubMenu->clear();

    if(bezierRoot->item != NULL)
    for(unsigned int i = 0;i < item->childCount();i++){
        auto* childItem = item->child(i);
        std::string text = childItem->text(0).toStdString();

        addToBezierSubMenu->addAction(text,
                                      SCENE_MENU_CHANGE_NAME_HANDLER);
    }

    ContextMenu::show(pos);
}
