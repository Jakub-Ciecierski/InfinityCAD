//
// Created by jakub on 3/5/16.
//

#ifndef MG1_SCENE_ID_FACTORY_H
#define MG1_SCENE_ID_FACTORY_H

#include "infinity_cad/rendering/scene/scene_id.h"

class SceneIDFactory {
private:

    unsigned int nextAvailableIDKeyValue;

public:

    SceneIDFactory();

    SceneID createNextAvailableID();

    SceneID getNextAvailableID();
};


#endif //MG1_SCENE_ID_FACTORY_H
