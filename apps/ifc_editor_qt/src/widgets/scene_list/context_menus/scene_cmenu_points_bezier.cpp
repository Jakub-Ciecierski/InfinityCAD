#include <widgets/scene_list/context_menus/scene_cmenu_points_bezier.h>
#include "system/object_manager.h"
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

using namespace std;

SceneCMenuPointsBezier::SceneCMenuPointsBezier()
{
    init();
}

void SceneCMenuPointsBezier::init(){
    this->addHandlerAndAction(SCENE_MENU_REMOVE_HANDLER);
}
