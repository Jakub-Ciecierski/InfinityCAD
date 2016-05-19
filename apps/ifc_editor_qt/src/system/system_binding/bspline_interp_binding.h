#ifndef BSPLINE_INTERP_BINDING_H
#define BSPLINE_INTERP_BINDING_H

#include <widgets/scene_list/scene_tree.h>
#include <infinity_cad/rendering/scene/scene.h>

class BSplineInterpBinding
{
private:
    Scene* scene;
    SceneTree* sceneTree;
public:
    BSplineInterpBinding(Scene* scene, SceneTree* sceneTree);

    Item* createBSplineInterp(std::string name);
};

#endif // BSPLINE_INTERP_BINDING_H
