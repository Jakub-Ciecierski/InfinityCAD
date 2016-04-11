#include "widgets/objects_list/context_menus/objects_cmenu_settings.h"
#include "system/ifc_types.h"
#include "editor_window.h"
#include "ui_mainwindow.h"

#include <gm/scene/scene.h>

#include "system/object_manager.h"


using namespace std;

const string OBJECTS_MENU_ADD_NAME = "Add";
const string OBJECTS_MENU_ADD_NAMED_NAME = "Add Named";
const string OBJECTS_MENU_PROPERTIES_NAME = "Properties";

Handler OBJECTS_MENU_ADD_HANDLER([](string objectType){
    ObjectManager& objManager = ObjectManager::getInstance();

    objManager.addObject(objectType);
});

Handler OBJECTS_MENU_ADD_NAMED_HANDLER([](string objectType){
    ObjectManager& objManager = ObjectManager::getInstance();

    string title = "New Object";
    string text = "Input name";
    string name = EditorWindow::getInstance().showInputBox(title, text);
    if(name.empty()) return;

    Type type = typeFromString(objectType);
    objManager.addObject(type, name);
});

Handler OBJECTS_MENU_PROPERTIES_HANDLER;
