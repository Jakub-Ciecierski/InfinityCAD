//
// Created by jakub on 3/5/16.
//

#include <gm/scene/scene_id.h>

SceneID::SceneID(unsigned int key) : key(key){

}

unsigned int SceneID::getKey() {
    return this->key;
}
