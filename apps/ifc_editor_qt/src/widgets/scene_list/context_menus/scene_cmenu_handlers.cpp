#include "scene_cmenu_handlers.h"
#include <system/object_manager.h>
#include <gm/rendering/render_bodies/curves/bezier_curve.h>

SceneCMHandler SCENE_MENU_MOVE_CROSS_HANDLER(
        "Move Cross",
     [](Item* objectName){
         ObjectManager& objManager = ObjectManager::getInstance();
         objManager.moveCross(objectName);
     });

SceneCMHandler SCENE_MENU_MOVE_CAMERA_HANDLER(
        "Move Camera",
        [](Item* objectName){
            ObjectManager& objManager = ObjectManager::getInstance();
            objManager.moveCamera(objectName);
        });

SceneCMHandler SCENE_MENU_DELETE_HANDLER(
        "Delete",
        [](Item* objectName){
    ObjectManager& objManager = ObjectManager::getInstance();
    objManager.deleteObject(objectName);
});

SceneCMHandler SCENE_MENU_CHANGE_NAME_HANDLER(
        "Change Name",
        [](Item* objectName){
    ObjectManager& objManager = ObjectManager::getInstance();
    objManager.changeName(objectName);
});

SceneCMHandler SCENE_MENU_POLYGON_HANDLER(
        "Show/Hide Polygon",
        [](Item* item){
    BezierCurve* bezierCurve = static_cast<BezierCurve*>(item->object);
    bezierCurve->setDrawBezierPolygon(!(bezierCurve->isDrawBezierPolygon()));
});


SceneCMHandler SCENE_MENU_MOVEUP_HANDLER(
        "Move up",
        [](Item* objectItem){
    ObjectManager& objManager = ObjectManager::getInstance();
    objManager.moveUpItem(objectItem);
});

SceneCMHandler SCENE_MENU_MOVEDOWN_HANDLER(
        "Move down",
        [](Item* objectItem){
    ObjectManager& objManager = ObjectManager::getInstance();
    objManager.moveDownItem(objectItem);

});

SceneCMHandler SCENE_MENU_REMOVE_HANDLER(
        "Disconnect",
        [](Item* objectItem){
    ObjectManager& objManager = ObjectManager::getInstance();
    objManager.removeChildItem(objectItem);
});

SceneCMHandler SCM_SWITCH_SPLINE_BASIS_HANDLER(
        "Bezier/B-Spline Basis",
        [](Item* item){

});
