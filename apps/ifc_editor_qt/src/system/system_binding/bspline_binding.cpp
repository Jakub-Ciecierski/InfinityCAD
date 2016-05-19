#include "bspline_binding.h"
#include <system/object_manager.h>
#include <infinity_cad/rendering/scene/object_factory.h>

BSplineBinding::BSplineBinding(Scene* scene, SceneTree* sceneTree) {
    this->scene = scene;
    this->sceneTree = sceneTree;
}

Item * BSplineBinding::createBSpline(std::string name){
    ObjectManager& objManager = ObjectManager::getInstance();
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderObject * t = objectFactory.createBSpline(name);

    this->scene->addRenderObject(t);
    Item* bezierItem = sceneTree->addObject(t, RB_BSPLINE_TYPE);

    // Add all selected points to curve
    std::vector<Item*> selectedPointItems =
            sceneTree->getSelectedItems(RB_POINT_TYPE);
    for(unsigned int i = 0; i < selectedPointItems.size(); i++){
        objManager.addChildItem(bezierItem, selectedPointItems[i]);
    }

    return bezierItem;
}

