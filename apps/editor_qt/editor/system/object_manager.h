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

    void deleteObject(std::string name);
    void changeName(std::string srcName);
    void moveCross(std::string srcName);
    void moveCamera(std::string objectName);

    void setActive(SceneID id);
    void setDeactive(SceneID id);
};

#endif // OBJECTMANAGER_H
