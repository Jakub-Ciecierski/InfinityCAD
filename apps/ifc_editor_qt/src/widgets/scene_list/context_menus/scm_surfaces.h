#ifndef SCM_SURFACES_H
#define SCM_SURFACES_H

#include <context_menus/scene_context_menu.h>
#include "context_menus/handler.h"
#include "scene_cmenu_handlers.h"

class SCMSurfaces : public SceneContextMenu
{
private:
    void init();
public:
    SCMSurfaces();
};

#endif // SCM_SURFACES_H
