//
// Created by jakub on 4/3/16.
//

#include <gm/rendering/render_bodies/primitivies/point.h>
#include <bits/stringfwd.h>
#include <gm/scene/object_factory.h>


//-----------------------//
//  CONSTRUCTORS
//-----------------------//

ObjectFactory::ObjectFactory(){

}

ObjectFactory::~ObjectFactory() {

}

//-----------------------//
//  PRIVATE 
//-----------------------//

//-----------------------//
//  PUBLIC
//-----------------------//


Point* ObjectFactory::createPoint(std::string name) {
    SceneID id = sceneIDFactory.createNextAvailableID();
    //Point* point = new Point();
    return nullptr;
}

Torus *ObjectFactory::createTorus(std::string name) {
    return nullptr;
}
