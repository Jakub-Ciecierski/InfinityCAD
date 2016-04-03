#include "widgets/scene_list/context_menus/scene_cmenu_settings.h"
#include "system/ic_names.h"
#include "editor_window.h"
#include "ui_mainwindow.h"

#include <gm/scene/scene.h>

#include "system/object_manager.h"


using namespace std;

const string SCENE_MENU_MOVE_CROSS_NAME = "Move Cross";
const string SCENE_MENU_MOVE_CAMERA_NAME = "Move Camera";
const string SCENE_MENU_DELETE_NAME = "Delete";
const string SCENE_MENU_CHANGE_NAME_NAME = "Change Name";

Handler SCENE_MENU_MOVE_CROSS_HANDLER([](string objectName){
    ObjectManager objManager = ObjectManager::getInstance();

    objManager.moveCross(objectName);
});

Handler SCENE_MENU_MOVE_CAMERA_HANDLER([](string objectName){
    ObjectManager objManager = ObjectManager::getInstance();

    objManager.moveCamera(objectName);
});

Handler SCENE_MENU_DELETE_HANDLER([](string objectName){
    ObjectManager objManager = ObjectManager::getInstance();

    objManager.deleteObject(objectName);
});

Handler SCENE_MENU_CHANGE_NAME_HANDLER([](string objectName){
    ObjectManager objManager = ObjectManager::getInstance();

    objManager.changeName(objectName);
});
