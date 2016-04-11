#include "scm_bspline.h"
#include <widgets/scene_list/context_menus/scene_cmenu_handlers.h>

SCMBSpline::SCMBSpline()
{
    init();
}

void SCMBSpline::init(){
    this->addHandlerAndAction(SCENE_MENU_MOVE_CROSS_HANDLER);

    this->addHandlerAndAction(SCENE_MENU_MOVE_CAMERA_HANDLER);

    this->addSeparator();

    this->addHandlerAndAction(SCENE_MENU_CHANGE_NAME_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_POLYGON_HANDLER);
    this->addHandlerAndAction(SCM_SWITCH_SPLINE_BASIS_HANDLER);

    this->addSeparator();
    this->addHandlerAndAction(SCENE_MENU_DELETE_HANDLER);
}
