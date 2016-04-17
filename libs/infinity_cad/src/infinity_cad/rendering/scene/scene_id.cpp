//
// Created by jakub on 3/5/16.
//

#include <infinity_cad/rendering/scene/scene_id.h>

SceneID::SceneID(){
    key = -1;
}

SceneID::SceneID(int key) : key(key){

}

int SceneID::getKey() const{
    return this->key;
}

bool SceneID::operator==(const SceneID& id) const{
    return (id.key == this->key);
}