#include "scene_cmenu_factory.h"

#include "context_menus/context_menu_settings.h"

SceneCMenuFactory::SceneCMenuFactory()
{

}

ContextMenu* SceneCMenuFactory::create(){
    ContextMenu* sceneListContextMenu = new ContextMenu();

    sceneListContextMenu->addAction((SCENE_MENU_MOVE_CROSS_NAME),
                                   SCENE_MENU_MOVE_CROSS_HANDLER);

    sceneListContextMenu->addAction((SCENE_MENU_MOVE_CAMERA_NAME),
                                   SCENE_MENU_MOVE_CAMERA_HANDLER);

    sceneListContextMenu->addSeparator();

    sceneListContextMenu->addAction((SCENE_MENU_CHANGE_NAME_NAME),
                                   SCENE_MENU_CHANGE_NAME_HANDLER);

    sceneListContextMenu->addSeparator();
    sceneListContextMenu->addAction((SCENE_MENU_DELETE_NAME),
                                   SCENE_MENU_DELETE_HANDLER);



    return sceneListContextMenu;
}
