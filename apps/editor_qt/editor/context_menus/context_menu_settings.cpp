#include "context_menus/context_menu_settings.h"
#include "widgets/objects_list/objects_settings.h"
#include "editor_window.h"
#include "ui_mainwindow.h"

#include <gm/scene/scene.h>

#include "system/object_manager.h"


using namespace std;

const string OBJECTS_MENU_ADD_NAME = "Add";
const string OBJECTS_MENU_ADD_NAMED_NAME = "Add Named";
const string OBJECTS_MENU_PROPERTIES_NAME = "Properties";

const string SCENE_MENU_MOVE_CROSS_NAME = "Move Cross";
const string SCENE_MENU_MOVE_CAMERA_NAME = "Move Camera";
const string SCENE_MENU_DELETE_NAME = "Delete";
const string SCENE_MENU_CHANGE_NAME_NAME = "Change Name";

Handler OBJECTS_MENU_ADD_HANDLER([](string objectType){
    ObjectManager objManager = ObjectManager::getInstance();

    objManager.addObject(objectType);
});

Handler OBJECTS_MENU_ADD_NAMED_HANDLER([](string objectType){
    ObjectManager objManager = ObjectManager::getInstance();

    string title = "New Object";
    string text = "Input name";
    string name = EditorWindow::getInstance().showInputBox(title, text);
    if(name.empty()) return;

    objManager.addObject(objectType, name);
});

Handler OBJECTS_MENU_PROPERTIES_HANDLER;

// ---------------------------------------------------------

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
