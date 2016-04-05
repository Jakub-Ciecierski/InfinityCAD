#ifndef SCENECMENUPOINTBEZIER_H
#define SCENECMENUPOINTBEZIER_H

#include <context_menus/scene_context_menu.h>
#include <widgets/scene_list/scene_tree.h>

class SceneCMenuPointBezier : public SceneContextMenu
{
private:
    void init();
    void initHandlers();

    Item* bezierRoot;

    std::string SCENE_MENU_MOVEUP_NAME;
    std::string SCENE_MENU_MOVEDOWN_NAME;
    std::string SCENE_MENU_REMOVE_NAME;


    SceneCMHandler SCENE_MENU_MOVEUP_HANDLER;
    SceneCMHandler SCENE_MENU_MOVEDOWN_HANDLER;
    SceneCMHandler SCENE_MENU_REMOVE_HANDLER;

public:
    SceneCMenuPointBezier();
};

#endif // SCENECMENUPOINTBEZIER_H
