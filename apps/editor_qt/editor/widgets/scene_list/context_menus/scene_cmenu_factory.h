#ifndef SCENECMENUFACTORY_H
#define SCENECMENUFACTORY_H

#include <context_menus/scene_context_menu.h>
#include <widgets/scene_list/context_menus/scene_cmenu_default.h>
#include <widgets/scene_list/context_menus/scene_cmenu_point.h>
#include <widgets/scene_list/context_menus/scene_cmenu_points.h>
#include <widgets/scene_list/context_menus/scene_cmenu_points_bezier.h>
#include <widgets/scene_list/context_menus/scene_cmenu_point_bezier.h>
#include <widgets/scene_list/context_menus/scene_cmenu_bezier.h>
#include <widgets/scene_list/entities/item.h>
#include <functional>

class SceneCMenuFactory
{
private:
    SceneCMenuFactory();

    SceneCMenuDefault* defaultMenu;

    SceneCMenuPoint* pointMenu;
    SceneCMenuPoints* pointsMenu;

    SceneCMenuPointBezier* pointBezierMenu;
    SceneCMenuPointsBezier* pointsBezierMenu;

    SceneCMenuBezier* bezierMenu;

public:
    ~SceneCMenuFactory();
    static SceneCMenuFactory& getInstance();

    SceneContextMenu* create();

    SceneContextMenu* getDefaultMenu();
    SceneContextMenu* getPointMenu(Item* bezierRoot);
    SceneContextMenu* getPointsMenu(Item* bezierRoot);
    
    int getSelectedTypeCount( const QList<QTreeWidgetItem *>& selectedItems,
                              const Type& type,
                              std::function<Item*(QTreeWidgetItem*)> getItemByTree);
    SceneContextMenu* getProperMenu(const QList<QTreeWidgetItem *>& selectedItems,
                               std::function<Item*(QTreeWidgetItem*)> getItemByTree,
                               std::function<Item*(const Type&)> getRootItem);
};

#endif // SCENECMENUFACTORY_H
