//
// Created by jakub on 4/3/16.
//

#include <infinity_cad/object.h>

using namespace ifc;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Object::Object(SceneID id){
    this->id = id;
    this->name = "Object";

    objectType = OBJ_TYPE_UNKNOWN;
}

Object::Object(SceneID id, std::string name){
    this->id = id;
    this->name = name;

    objectType = OBJ_TYPE_UNKNOWN;
}

Object::~Object(){

}

//-----------------------//
//  PUBLIC 
//-----------------------//

void Object::setName(std::string name){
    this->name = name;
}

std::string Object::getName(){
    return this->name;
}

const SceneID& Object::getID(){
    return this->id;
}

const ObjectType& Object::getType(){
    return this->objectType;
}