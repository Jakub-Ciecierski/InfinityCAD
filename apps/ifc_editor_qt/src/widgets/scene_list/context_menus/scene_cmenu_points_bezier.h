#ifndef SCENECMENUPOINTSBEZIER_H
#define SCENECMENUPOINTSBEZIER_H

#include <context_menus/scene_context_menu.h>
#include <widgets/scene_list/scene_tree.h>

class SceneCMenuPointsBezier : public SceneContextMenu
{
private:
    void init();

    Item* bezierRoot;

public:
    SceneCMenuPointsBezier();
};

#endif // SCENECMENUPOINTSBEZIER_H
