#ifndef SCM_SURFACES_FILL_H
#define SCM_SURFACES_FILL_H

#include <context_menus/scene_context_menu.h>
#include "context_menus/handler.h"
#include "scene_cmenu_handlers.h"

class SCMSurfacesFill : public SceneContextMenu
{
private:
    void init();

public:
    SCMSurfacesFill();
};

#endif // SCM_SURFACES_FILL_H
