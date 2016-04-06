#ifndef SCENECMENUBEZIER_H
#define SCENECMENUBEZIER_H

#include <context_menus/scene_context_menu.h>
#include "context_menus/handler.h"

class SceneCMenuBezier : public SceneContextMenu
{
private:
    void init();
    void initHandlers();

    std::string SCENE_MENU_MOVE_CROSS_NAME;
    std::string SCENE_MENU_MOVE_CAMERA_NAME;
    std::string SCENE_MENU_DELETE_NAME;
    std::string SCENE_MENU_CHANGE_NAME_NAME;
    std::string SCENE_MENU_POLYGON_NAME;

    SceneCMHandler SCENE_MENU_MOVE_CROSS_HANDLER;
    SceneCMHandler SCENE_MENU_MOVE_CAMERA_HANDLER;
    SceneCMHandler SCENE_MENU_DELETE_HANDLER;
    SceneCMHandler SCENE_MENU_CHANGE_NAME_HANDLER;
    SceneCMHandler SCENE_MENU_POLYGON_HANDLER;

public:
    SceneCMenuBezier();
};

#endif // SCENECMENUBEZIER_H
