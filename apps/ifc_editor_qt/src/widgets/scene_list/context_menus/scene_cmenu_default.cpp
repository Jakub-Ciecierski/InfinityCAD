#include <widgets/scene_list/context_menus/scene_cmenu_default.h>
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

using namespace std;

SceneCMenuDefault::SceneCMenuDefault(){
    init();
}

void SceneCMenuDefault::init(){
    this->addHandlerAndAction(SCENE_MENU_MOVE_CROSS_HANDLER);

    this->addHandlerAndAction(SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();

    this->addHandlerAndAction(SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_DELETE_HANDLER);
}
