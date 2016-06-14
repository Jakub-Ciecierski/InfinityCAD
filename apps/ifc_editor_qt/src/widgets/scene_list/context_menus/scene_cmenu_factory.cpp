#include "widgets/scene_list/context_menus/scene_cmenu_factory.h"
#include <QTreeWidget>

using namespace std;

SceneCMenuFactory::SceneCMenuFactory() {
    defaultMenu = new SceneCMenuDefault();

    pointMenu = new SceneCMenuPoint();
    pointsMenu = new SceneCMenuPoints();

    pointBezierMenu = new SceneCMenuPointBezier();
    pointsBezierMenu = new SceneCMenuPointsBezier();

    bezierMenu = new SceneCMenuBezier();

    bsplineMenu = new SCMBSpline();

    surfaceMenu = new SCMSurfaceC0();
    surfacesMenu = new SCMSurfaces();
}

SceneCMenuFactory::~SceneCMenuFactory() {
    delete defaultMenu;

    delete pointMenu;
    delete pointsMenu;

    delete pointBezierMenu;
    delete pointsBezierMenu;

    delete bezierMenu;

    delete bsplineMenu;

    delete surfaceMenu;
    delete surfacesMenu;
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
                                                RB_POINT_CLONE_TYPE,
                                                getItemByTree);

    int bezierCount = getSelectedTypeCount(selectedItems,
                                           RB_BEZIER_TYPE,
                                           getItemByTree);

    int bsplineCount = getSelectedTypeCount(selectedItems,
                                            RB_BSPLINE_TYPE,
                                            getItemByTree);

    int surfaceCount = getSelectedTypeCount(selectedItems,
                                            RB_SURFACE_C0_CYLIND_TYPE,
                                            getItemByTree);
    surfaceCount += getSelectedTypeCount(selectedItems,
                                            RB_SURFACE_C0_RECT_TYPE,
                                            getItemByTree);
    surfaceCount += getSelectedTypeCount(selectedItems,
                                            RB_SURFACE_C2_RECT_TYPE,
                                            getItemByTree);
    surfaceCount += getSelectedTypeCount(selectedItems,
                                            RB_SURFACE_C2_CYLIND_TYPE,
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
    }else if (bezierCount == totalCount){
        if(bezierCount == 1){
            return this->bezierMenu;
        }
    }else if(bsplineCount == totalCount){
        return this->bsplineMenu;
    }else if(surfaceCount == totalCount){

        if(totalCount == 1)
            return surfaceMenu;

        if(totalCount == 2){
            return surfacesMenu;
        }
    }

    if(totalCount == 1){
        return this->getDefaultMenu();
    }

    return NULL;
}
