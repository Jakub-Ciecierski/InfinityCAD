#ifndef SCENECMENUPOINTBEZIER_H
#define SCENECMENUPOINTBEZIER_H

#include <context_menus/scene_context_menu.h>
#include <widgets/scene_list/scene_tree.h>

class SceneCMenuPointBezier : public SceneContextMenu
{
private:
    void init();

    Item* bezierRoot;

public:
    SceneCMenuPointBezier();
};

#endif // SCENECMENUPOINTBEZIER_H
