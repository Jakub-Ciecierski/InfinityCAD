#ifndef SCENECMENUDEFAULT_H
#define SCENECMENUDEFAULT_H

#include <context_menus/scene_context_menu.h>
#include "context_menus/handler.h"

class SceneCMenuDefault : public SceneContextMenu
{
private:
    void init();
    void initHandlers();

    std::string SCENE_MENU_MOVE_CROSS_NAME;
    std::string SCENE_MENU_MOVE_CAMERA_NAME;
    std::string SCENE_MENU_DELETE_NAME;
    std::string SCENE_MENU_CHANGE_NAME_NAME;

    SceneCMHandler SCENE_MENU_MOVE_CROSS_HANDLER;
    SceneCMHandler SCENE_MENU_MOVE_CAMERA_HANDLER;
    SceneCMHandler SCENE_MENU_DELETE_HANDLER;
    SceneCMHandler SCENE_MENU_CHANGE_NAME_HANDLER;

public:
    SceneCMenuDefault();

};

#endif // SCENECMENUDEFAULT_H
