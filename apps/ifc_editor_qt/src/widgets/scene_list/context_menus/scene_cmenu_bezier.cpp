#include <gm/rendering/render_bodies/curves/bezier_curve.h>
#include <widgets/scene_list/context_menus/scene_cmenu_bezier.h>
#include "system/object_manager.h"

SceneCMenuBezier::SceneCMenuBezier()
{
    init();
}

void SceneCMenuBezier::init(){
    initHandlers();

    this->addActionAndHandler((SCENE_MENU_MOVE_CROSS_NAME),
                              SCENE_MENU_MOVE_CROSS_HANDLER);

    this->addActionAndHandler((SCENE_MENU_MOVE_CAMERA_NAME),
                              SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();

    this->addActionAndHandler((SCENE_MENU_CHANGE_NAME_NAME),
                              SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addActionAndHandler((SCENE_MENU_POLYGON_NAME),
                              SCENE_MENU_POLYGON_HANDLER);

    this->addSeparator();
    this->addActionAndHandler((SCENE_MENU_DELETE_NAME),
                              SCENE_MENU_DELETE_HANDLER);
}

void SceneCMenuBezier::initHandlers(){
    SCENE_MENU_MOVE_CROSS_NAME = "Move Cross";
    SCENE_MENU_MOVE_CROSS_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.moveCross(objectName);
    });

    SCENE_MENU_MOVE_CAMERA_NAME = "Move Camera";
    SCENE_MENU_MOVE_CAMERA_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.moveCamera(objectName);
    });

    SCENE_MENU_CHANGE_NAME_NAME = "Change Name";
    SCENE_MENU_CHANGE_NAME_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.changeName(objectName);
    });

    SCENE_MENU_DELETE_NAME = "Delete";
    SCENE_MENU_DELETE_HANDLER = SceneCMHandler([](Item* objectName){
        ObjectManager objManager = ObjectManager::getInstance();
        objManager.deleteObject(objectName);
    });

    SCENE_MENU_POLYGON_NAME = "Show/Hide Polygon";
    SCENE_MENU_POLYGON_HANDLER = SceneCMHandler([](Item* item){
            BezierCurve* bezierCurve = static_cast<BezierCurve*>(item->object);
            bezierCurve->setDrawBezierPolygon(!(bezierCurve->isDrawBezierPolygon()));
    });

}

