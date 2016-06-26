#include "scene_cmenu_handlers.h"
#include <system/object_manager.h>
#include <infinity_cad/rendering/render_objects/curves/spline.h>
#include <infinity_cad/rendering/render_objects/curves/bspline.h>
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

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
    Spline* spline = static_cast<Spline*>(item->object);
    bool value = spline->isDrawBezierPolygon();
    spline->setDrawBezierPolygon(!value);
});

SceneCMHandler SCENE_MENU_POLYGON_SURFACE_HANDLER(
        "Show/Hide Polygon",
        [](Item* item){
    Surface* surface = static_cast<Surface*>(item->object);
    bool value = surface->isDrawPolygon();
    surface->setDrawPolygon(!value);
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
    BSpline* bSpline = static_cast<BSpline*>(item->object);
    bSpline->setDrawBezierBasis(!(bSpline->isDrawBezierBasis()));
});

SceneCMHandler SCM_INTERSECTION_SURFACE_HANDLER(
        "Find Intersection",
        [](Item* item){
   ObjectManager& objManager = ObjectManager::getInstance();
   objManager.runSurfaceIntersection();
});

SceneCMHandler SCM_INTERSECTION_CLICK_SURFACE_HANDLER(
        "Find Intersection with Click",
        [](Item* item){
   ObjectManager& objManager = ObjectManager::getInstance();
   objManager.runSurfaceIntersectionWithClick();
});

SceneCMHandler SCM_FILLING_SURFACE_HANDLER(
        "Fill the hole",
        [](Item* item){
    ObjectManager& objManager = ObjectManager::getInstance();
    objManager.runFillingPatch();
});
