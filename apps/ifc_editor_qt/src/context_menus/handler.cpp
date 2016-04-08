#include <context_menus/handler.h>

using namespace std;

Handler::Handler(){
    handler = [](string){ };
}

Handler::Handler(function<void(string)> handler):
    handler(handler)
{

}

void Handler::execute(string objectName){
    handler(objectName);
}
