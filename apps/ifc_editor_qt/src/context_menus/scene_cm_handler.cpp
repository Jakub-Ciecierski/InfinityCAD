#include <context_menus/scene_cm_handler.h>

using namespace std;

SceneCMHandler::SceneCMHandler(){
    handler = [](Item* item){ };
}

SceneCMHandler::SceneCMHandler(function<void(Item*)> handler):
    handler(handler) {

}

void SceneCMHandler::execute(Item* objectID){
    handler(objectID);
}
