#include "widgets/scene_list/context_menus/scene_cmenu_factory.h"
#include "widgets/scene_list/context_menus/scene_cmenu_settings.h"


SceneCMenuFactory::SceneCMenuFactory() {
    defaultMenu = new SceneCMenuDefault();
    pointMenu = new SceneCMenuPoint();
}

SceneCMenuFactory::~SceneCMenuFactory() {
    delete defaultMenu;
    delete pointMenu;
}

SceneCMenuFactory& SceneCMenuFactory::getInstance(){
    static SceneCMenuFactory fac;

    return fac;
}

ContextMenu* SceneCMenuFactory::getDefaultMenu(){
    return this->defaultMenu;
}

ContextMenu* SceneCMenuFactory::getPointMenu(RootItem* bezierRoot){
    this->pointMenu->setBezierRoot(bezierRoot);

    return this->pointMenu;
}
