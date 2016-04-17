#include <widgets/scene_list/context_menus/scene_cmenu_bezier.h>
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

#include "system/object_manager.h"

SceneCMenuBezier::SceneCMenuBezier()
{
    init();
}

void SceneCMenuBezier::init(){
    this->addHandlerAndAction(SCENE_MENU_MOVE_CROSS_HANDLER);

    this->addHandlerAndAction(SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();

    this->addHandlerAndAction(SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_POLYGON_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_DELETE_HANDLER);
}
