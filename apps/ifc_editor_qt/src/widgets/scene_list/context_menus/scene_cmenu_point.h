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

    Item* bezierRoot;
    SceneContextMenu* addToBezierSubMenu;

public:
    SceneCMenuPoint();

    void setBezierRoot(Item* bezierRoot);

    virtual QAction* show(const QPoint& pos) override;
};

#endif // SCENECMENUPOINT_H
