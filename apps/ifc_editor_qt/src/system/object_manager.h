#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "glwidget.h"
#include <infinity_cad/rendering/render_objects/primitivies/point.h>
#include "widgets/scene_list/scene_tree.h"
#include <system/system_binding/bspline_binding.h>
#include <system/system_binding/bspline_interp_binding.h>
#include <string>
#include <math/matrix.h>

class ObjectManager
{
private:
    ObjectManager();

    BSplineBinding* bSplineBinding;
    BSplineInterpBinding* bSplineInterpBinding;

    GLWidget* glWidget;
    Scene* scene;
    SceneTree* sceneTree;

    std::string getDefaultName(const Type& type);

public:
    ~ObjectManager();

    static ObjectManager& getInstance();

    Item* addObject(const Type& type);
    Item* addObject(const Type& type, std::string name);

    Item * addTorus(std::string name);
    Item * addPoint(std::string name);
    Item * addBezierCurve(std::string name);

    Item* addSurfaceC0Rect(std::string name);
    Item* addSurfaceC0Rect(std::string name,
                           Matrix<ifc::Point*> pointsMatrix,
                           std::vector<Item*> pointItems);

    Item* addSurfaceC0Cylind(std::string name);
    Item* addSurfaceC0Cylind(std::string name,
                             Matrix<ifc::Point*> pointsMatrix,
                             std::vector<Item*> pointItems);

    Item* addSurfaceC2Rect(std::string name);
    Item* addSurfaceC2Rect(std::string name,
                           Matrix<ifc::Point*> points,
                           std::vector<Item*> pointItems);

    Item* addSurfaceC2Cylind(std::string name);
    Item* addSurfaceC2Cylind(std::string name,
                             Matrix<ifc::Point*> pointsMatrix,
                             std::vector<Item*> pointItems);

    // TODO split
    void addChildItem(Item* bezier, Item* objectName);
    void removeChildItem(Item* pointItem);
    void moveUpItem(Item* pointItem);
    void moveDownItem(Item* pointItem);

    void deleteObject(Item* name);
    void changeName(Item* srcName);
    void moveCross(Item* srcName);
    void moveCamera(Item* objectName);

    void setActive(SceneID id);
    void setDeactive(SceneID id);

    void saveSystem(std::string filepath);
    void loadSystem(std::string filepath);

    SceneTree* getSceneTree();

    void colapsSelectedPoints();

    void TEST_BSPLINE();
    void TEST_PERFORMANCE();
    void TEST_BSPLINE_INTERP();
    void TEST_SURFACE();
};

#endif // OBJECTMANAGER_H
