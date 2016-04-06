#include "scene_cmenu_point_bezier.h"
#include <system/object_manager.h>

using namespace std;

SceneCMenuPointBezier::SceneCMenuPointBezier()
{
    init();
}

void SceneCMenuPointBezier::init(){
    initHandlers();

    this->addActionAndHandler((SCENE_MENU_MOVEUP_NAME),
                              SCENE_MENU_MOVEUP_HANDLER);
    this->addActionAndHandler((SCENE_MENU_MOVEDOWN_NAME),
                              SCENE_MENU_MOVEDOWN_HANDLER);
    this->addSeparator();
    this->addActionAndHandler((SCENE_MENU_REMOVE_NAME),
                              SCENE_MENU_REMOVE_HANDLER);
}

void SceneCMenuPointBezier::initHandlers(){
    SCENE_MENU_MOVEUP_NAME = "Move up";
    SCENE_MENU_MOVEUP_HANDLER = SceneCMHandler([](Item* objectItem){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.movePointUpBezier(objectItem);
    });

    SCENE_MENU_MOVEDOWN_NAME = "Move down";
    SCENE_MENU_MOVEDOWN_HANDLER = SceneCMHandler([](Item* objectItem){
            ObjectManager objManager = ObjectManager::getInstance();
            objManager.movePointDownBezier(objectItem);

    });

    SCENE_MENU_REMOVE_NAME = "Disconnect";
    SCENE_MENU_REMOVE_HANDLER = SceneCMHandler([](Item* objectItem){
            ObjectManager& objManager = ObjectManager::getInstance();
            objManager.removePointFromBezier(objectItem);
    });

}
