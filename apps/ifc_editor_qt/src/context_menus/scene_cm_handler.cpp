#include <context_menus/scene_cm_handler.h>

using namespace std;

SceneCMHandler::SceneCMHandler(){
    handler = [](Item* item){ if(item != NULL) return; };
}

SceneCMHandler::SceneCMHandler(function<void(Item*)> handler):
    handler(handler) {

}

SceneCMHandler::SceneCMHandler(std::string name,
                               std::function<void(Item*)> handler) :
    name(name), handler(handler){

}

void SceneCMHandler::execute(Item* objectID){
    handler(objectID);
}

std::string SceneCMHandler::getName(){
    return this->name;
}
