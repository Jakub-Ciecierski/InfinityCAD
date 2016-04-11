#include <widgets/scene_list/context_menus/scene_cmenu_point_bezier.h>
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

using namespace std;

SceneCMenuPointBezier::SceneCMenuPointBezier()
{
    init();
}

void SceneCMenuPointBezier::init(){
    this->addHandlerAndAction(SCENE_MENU_MOVEUP_HANDLER);
    this->addHandlerAndAction(SCENE_MENU_MOVEDOWN_HANDLER);
    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_REMOVE_HANDLER);
}
