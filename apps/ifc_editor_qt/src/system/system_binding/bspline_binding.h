#ifndef BSPLINEBINDING_H
#define BSPLINEBINDING_H

#include <gm/scene/scene.h>
#include <widgets/scene_list/scene_tree.h>

class BSplineBinding
{
private:
    Scene* scene;
    SceneTree* sceneTree;
public:
    BSplineBinding(Scene* scene, SceneTree* sceneTree);

    RenderBody* createBSpline(std::string name);
};

#endif // BSPLINEBINDING_H
