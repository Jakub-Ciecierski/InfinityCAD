#ifndef SCENECMENUPOINTSBEZIER_H
#define SCENECMENUPOINTSBEZIER_H

#include <context_menus/scene_context_menu.h>
#include <widgets/scene_list/scene_tree.h>

class SceneCMenuPointsBezier : public SceneContextMenu
{
private:
    void init();
    void initHandlers();

    Item* bezierRoot;

    std::string SCENE_MENU_REMOVE_NAME;

    SceneCMHandler SCENE_MENU_REMOVE_HANDLER;

public:
    SceneCMenuPointsBezier();
};

#endif // SCENECMENUPOINTSBEZIER_H
