//
// Created by jakub on 3/5/16.
//

#include <infinity_cad/rendering/scene/scene_id_factory.h>

SceneIDFactory::SceneIDFactory() {

}

SceneID SceneIDFactory::createNextAvailableID() {
    return SceneID(nextAvailableIDKeyValue++);
}

SceneID SceneIDFactory::getNextAvailableID() {
    return SceneID(nextAvailableIDKeyValue);
}