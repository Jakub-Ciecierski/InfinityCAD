#ifndef SCENECMENUPOINTS_H
#define SCENECMENUPOINTS_H

#include <context_menus/scene_context_menu.h>
#include <widgets/scene_list/scene_tree.h>

/*
 * Menu for when only multiple points are selected
 * */
class SceneCMenuPoints : public SceneContextMenu
{
private:
    void init();
    void initHandlers();

    Item* bezierRoot;
    SceneContextMenu* addToBezierSubMenu;

public:
    SceneCMenuPoints();

    void setBezierRoot(Item* bezierRoot);

    virtual QAction* show(const QPoint& pos) override;
};


#endif // SCENECMENUPOINTS_H
