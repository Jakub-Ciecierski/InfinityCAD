#include "scm_gregory.h"

SCMGregory::SCMGregory() : SceneContextMenu()
{
    init();
}

void SCMGregory::init(){
    this->addHandlerAndAction(SCM_GREGORY_DEBUG_HANDLER);

    this->addSeparator();

    this->addHandlerAndAction(SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();

    this->addHandlerAndAction(SCENE_MENU_DELETE_HANDLER);
}
