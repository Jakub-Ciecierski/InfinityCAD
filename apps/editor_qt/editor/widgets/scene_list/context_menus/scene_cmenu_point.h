#ifndef SCENECMENUPOINT_H
#define SCENECMENUPOINT_H

#include <context_menus/context_menu.h>
#include <widgets/scene_list/scene_tree.h>

class SceneCMenuPoint : public ContextMenu
{
private:
    void init();
    void initHandlers();

    RootTreeItem* bezierRoot;
    ContextMenu* addToBezierSubMenu;

    std::string SCENE_MENU_MOVE_CROSS_NAME;
    std::string SCENE_MENU_MOVE_CAMERA_NAME;
    std::string SCENE_MENU_DELETE_NAME;
    std::string SCENE_MENU_CHANGE_NAME_NAME;
    std::string SCENE_MENU_BEZIER_NAME;

    Handler SCENE_MENU_MOVE_CROSS_HANDLER;
    Handler SCENE_MENU_MOVE_CAMERA_HANDLER;
    Handler SCENE_MENU_DELETE_HANDLER;
    Handler SCENE_MENU_CHANGE_NAME_HANDLER;

public:
    SceneCMenuPoint();

    void setBezierRoot(RootTreeItem* bezierRoot);

    virtual QAction* show(const QPoint& pos) override;
};

#endif // SCENECMENUPOINT_H
