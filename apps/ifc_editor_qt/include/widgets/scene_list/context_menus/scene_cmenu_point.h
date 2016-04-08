#ifndef SCENECMENUPOINT_H
#define SCENECMENUPOINT_H

#include <context_menus/scene_context_menu.h>
#include <widgets/scene_list/scene_tree.h>

/*
 * Menu for when only one point is selected
 * */
class SceneCMenuPoint : public SceneContextMenu
{
private:
    void init();
    void initHandlers();

    Item* bezierRoot;
    SceneContextMenu* addToBezierSubMenu;

    std::string SCENE_MENU_MOVE_CROSS_NAME;
    std::string SCENE_MENU_MOVE_CAMERA_NAME;
    std::string SCENE_MENU_DELETE_NAME;
    std::string SCENE_MENU_CHANGE_NAME_NAME;
    std::string SCENE_MENU_BEZIER_NAME;

    SceneCMHandler SCENE_MENU_MOVE_CROSS_HANDLER;
    SceneCMHandler SCENE_MENU_MOVE_CAMERA_HANDLER;
    SceneCMHandler SCENE_MENU_DELETE_HANDLER;
    SceneCMHandler SCENE_MENU_CHANGE_NAME_HANDLER;

public:
    SceneCMenuPoint();

    void setBezierRoot(Item* bezierRoot);

    virtual QAction* show(const QPoint& pos) override;
};

#endif // SCENECMENUPOINT_H
