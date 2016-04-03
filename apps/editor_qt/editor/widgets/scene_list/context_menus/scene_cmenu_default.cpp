#include "scene_cmenu_default.h"
#include "system/object_manager.h"

using namespace std;

SceneCMenuDefault::SceneCMenuDefault(){
    init();
}

void SceneCMenuDefault::init(){
    initHandlers();

    this->addAction((SCENE_MENU_MOVE_CROSS_NAME),
                    SCENE_MENU_MOVE_CROSS_HANDLER);

    this->addAction((SCENE_MENU_MOVE_CAMERA_NAME),
                    SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();

    this->addAction((SCENE_MENU_CHANGE_NAME_NAME),
                    SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addAction((SCENE_MENU_DELETE_NAME),
                    SCENE_MENU_DELETE_HANDLER);
}

void SceneCMenuDefault::initHandlers(){
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
}
