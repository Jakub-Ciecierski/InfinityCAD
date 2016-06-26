#include "scm_surfaces_fill.h"

SCMSurfacesFill::SCMSurfacesFill()
{
    init();
    runForOneSelectedItem = true;
}

void SCMSurfacesFill::init(){
    this->addSeparator();
    this->addHandlerAndAction(SCM_FILLING_SURFACE_HANDLER);
}
