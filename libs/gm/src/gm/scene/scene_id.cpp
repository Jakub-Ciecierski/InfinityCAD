//
// Created by jakub on 3/5/16.
//

#include <gm/scene/scene_id.h>

SceneID::SceneID(){
    key = -1;
}

SceneID::SceneID(int key) : key(key){

}

int SceneID::getKey() const{
    return this->key;
}
