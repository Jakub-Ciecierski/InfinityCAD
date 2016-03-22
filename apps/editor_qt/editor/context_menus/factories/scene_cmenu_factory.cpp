#include "scene_cmenu_factory.h"

#include "context_menus/context_menu_settings.h"

SceneCMenuFactory::SceneCMenuFactory()
{

}

ContextMenu* SceneCMenuFactory::create(){
    ContextMenu* sceneListContextMenu = new ContextMenu();

    sceneListContextMenu->addAction((SCENE_MENU_DELETE_NAME),
                                   SCENE_MENU_DELETE_HANDLER);


    sceneListContextMenu->addAction((SCENE_MENU_CHANGE_NAME_NAME),
                                   SCENE_MENU_CHANGE_NAME_HANDLER);

    return sceneListContextMenu;
}
