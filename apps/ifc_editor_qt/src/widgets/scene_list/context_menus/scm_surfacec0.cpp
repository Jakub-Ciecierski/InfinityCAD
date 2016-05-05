#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>
#include <widgets/scene_list/context_menus/scm_surfacec0.h>

SCMSurfaceC0::SCMSurfaceC0()
{
    init();
}

void SCMSurfaceC0::init(){
    this->addHandlerAndAction(SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_POLYGON_SURFACE_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_DELETE_HANDLER);
}
