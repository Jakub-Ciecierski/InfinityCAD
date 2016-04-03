#ifndef SCENECMENUFACTORY_H
#define SCENECMENUFACTORY_H

#include "context_menus/context_menu.h"
#include <widgets/scene_list/scene_tree.h>
#include <widgets/scene_list/context_menus/scene_cmenu_default.h>
#include <widgets/scene_list/context_menus/scene_cmenu_point.h>

class SceneCMenuFactory
{
private:
    SceneCMenuFactory();

    SceneCMenuDefault* defaultMenu;
    SceneCMenuPoint* pointMenu;

public:
    ~SceneCMenuFactory();
    static SceneCMenuFactory& getInstance();

    ContextMenu* create();

    ContextMenu* getDefaultMenu();
    ContextMenu* getPointMenu(RootTreeItem* bezierRoot);
};

#endif // SCENECMENUFACTORY_H
