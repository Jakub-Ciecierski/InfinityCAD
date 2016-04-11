#include <widgets/scene_list/context_menus/scene_cmenu_points_bezier.h>
#include "system/object_manager.h"
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

using namespace std;

SceneCMenuPointsBezier::SceneCMenuPointsBezier()
{
    init();
}

void SceneCMenuPointsBezier::init(){
    initHandlers();

    this->addHandlerAndAction(SCENE_MENU_REMOVE_HANDLER);
}

void SceneCMenuPointsBezier::initHandlers(){
    SCENE_MENU_REMOVE_NAME = "Disconnect";
    SCENE_MENU_REMOVE_HANDLER = SceneCMHandler([](Item* objectItem){
            ObjectManager& objManager = ObjectManager::getInstance();
            objManager.removeChildItem(objectItem);
    });

}
