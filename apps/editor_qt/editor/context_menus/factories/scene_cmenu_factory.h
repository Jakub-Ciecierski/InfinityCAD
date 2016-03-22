#ifndef SCENECMENUFACTORY_H
#define SCENECMENUFACTORY_H

#include "context_menus/context_menu.h"

class SceneCMenuFactory
{
public:
    SceneCMenuFactory();

    ContextMenu* create();
};

#endif // SCENECMENUFACTORY_H
