#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "glwidget.h"

#include "widgets/scene_list/scene_tree.h"
#include <system/system_binding/bspline_binding.h>
#include <string>

class ObjectManager
{
private:
    ObjectManager();

    BSplineBinding* bSplineBinding;

    GLWidget* glWidget;
    Scene* scene;
    SceneTree* sceneTree;

    RenderObject * addTorus(std::string name);
    RenderObject * addPoint(std::string name);
    RenderObject * addBezierCurve(std::string name);

    std::string getDefaultName(const Type& type);

public:
    ~ObjectManager();

    static ObjectManager& getInstance();

    void addObject(const Type& type);
    void addObject(const Type& type, std::string name);

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

    void TEST_BSPLINE();
    void TEST_PERFORMANCE();
};

#endif // OBJECTMANAGER_H
