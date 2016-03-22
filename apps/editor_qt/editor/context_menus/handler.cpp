#include "handler.h"

using namespace std;

Handler::Handler(){
    handler = [](string){ };
}

Handler::Handler(void (*handler)(string)):
    handler(handler)
{

}

void Handler::execute(string objectName){
    handler(objectName);
}
