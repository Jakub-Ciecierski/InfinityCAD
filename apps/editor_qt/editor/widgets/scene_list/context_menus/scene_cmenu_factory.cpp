#include "widgets/scene_list/context_menus/scene_cmenu_factory.h"
#include "widgets/scene_list/context_menus/scene_cmenu_settings.h"
#include <QTreeWidget>

using namespace std;

SceneCMenuFactory::SceneCMenuFactory() {
    defaultMenu = new SceneCMenuDefault();

    pointMenu = new SceneCMenuPoint();
    pointsMenu = new SceneCMenuPoints();

    pointBezierMenu = new SceneCMenuPointBezier();
    pointsBezierMenu = new SceneCMenuPointsBezier();
}

SceneCMenuFactory::~SceneCMenuFactory() {
    delete defaultMenu;

    delete pointMenu;
    delete pointsMenu;

    delete pointBezierMenu;
    delete pointsBezierMenu;
}

SceneCMenuFactory& SceneCMenuFactory::getInstance(){
    static SceneCMenuFactory fac;

    return fac;
}

SceneContextMenu* SceneCMenuFactory::getDefaultMenu(){
    return this->defaultMenu;
}

SceneContextMenu* SceneCMenuFactory::getPointMenu(Item* bezierRoot){
    this->pointMenu->setBezierRoot(bezierRoot);

    return this->pointMenu;
}

SceneContextMenu* SceneCMenuFactory::getPointsMenu(Item* bezierRoot){
    this->pointsMenu->setBezierRoot(bezierRoot);

    return this->pointsMenu;
}

int SceneCMenuFactory::getSelectedTypeCount(
        const QList<QTreeWidgetItem *>& selectedItems, const Type& type,
        function<Item*(QTreeWidgetItem*)> getItemByTree){
    int pointCount = 0;
    for(auto* treeItem : selectedItems){
        Item* item = getItemByTree(treeItem);
        if(item != NULL && item->type == type){
            pointCount++;
        }
    }
    return pointCount;
}

SceneContextMenu* SceneCMenuFactory::getProperMenu(
        const QList<QTreeWidgetItem *>& selectedItems,
        function<Item*(QTreeWidgetItem*)> getItemByTree,
        function<Item*(const Type&)> getRootItem){
    int totalCount = selectedItems.size();
    int pointCount = getSelectedTypeCount(selectedItems,
                                          RB_POINT_TYPE,
                                          getItemByTree);
    int pointBezierCount = getSelectedTypeCount(selectedItems,
                                                RB_POINT_BEZIER_TYPE,
                                                getItemByTree);
    if(pointCount == totalCount){
        Item* bezierRoot = getRootItem(RB_BEZIER_TYPE);
        if(bezierRoot == NULL)
            throw new std::invalid_argument("Bezier not implemented");
        if(pointCount == 1)
            return this->getPointMenu(bezierRoot);
        else
            return this->getPointsMenu(bezierRoot);
    }else if(pointBezierCount == totalCount){
        if(totalCount == 1)
            return this->pointBezierMenu;
        else
            return this->pointsBezierMenu;
    }

    return this->getDefaultMenu();
}
