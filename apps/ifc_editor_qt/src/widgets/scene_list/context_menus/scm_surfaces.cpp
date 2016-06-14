#include "scm_surfaces.h"

SCMSurfaces::SCMSurfaces() : SceneContextMenu()
{
    init();
    runForOneSelectedItem = true;
}

void SCMSurfaces::init(){
    this->addSeparator();
    this->addHandlerAndAction(SCM_INTERSECTION_SURFACE_HANDLER);
}
