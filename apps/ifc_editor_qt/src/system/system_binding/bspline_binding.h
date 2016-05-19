#ifndef BSPLINEBINDING_H
#define BSPLINEBINDING_H

#include <widgets/scene_list/scene_tree.h>
#include <infinity_cad/rendering/scene/scene.h>

class BSplineBinding
{
private:
    Scene* scene;
    SceneTree* sceneTree;
public:
    BSplineBinding(Scene* scene, SceneTree* sceneTree);

    Item * createBSpline(std::string name);
};

#endif // BSPLINEBINDING_H
