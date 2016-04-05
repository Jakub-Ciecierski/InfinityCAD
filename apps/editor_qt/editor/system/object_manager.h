#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "glwidget.h"
#include <gm/scene/scene.h>
#include <gm/scene/scene_id.h>
#include <string>
#include "widgets/scene_list/scene_tree.h"

class ObjectManager
{
private:
    ObjectManager();


    GLWidget* glWidget;
    Scene* scene;
    SceneTree* sceneTree;

    void addTorus(std::string name);
    void addPoint(std::string name);
    void addBezierCurve(std::string name);

    std::string getDefaultName(const Type& type);

public:
    static ObjectManager& getInstance();

    void addObject(const Type& type);
    void addObject(const Type& type, std::string name);

    void addPointToBezier(Item* bezier, Item* objectName);
    void deleteObject(Item* name);
    void changeName(Item* srcName);
    void moveCross(Item* srcName);
    void moveCamera(Item* objectName);

    void setActive(SceneID id);
    void setDeactive(SceneID id);
};

#endif // OBJECTMANAGER_H
