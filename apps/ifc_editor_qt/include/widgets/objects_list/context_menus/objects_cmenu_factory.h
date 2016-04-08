#ifndef OBJECTSCMENUFACTORY_H
#define OBJECTSCMENUFACTORY_H

#include "context_menus/context_menu.h"

class ObjectsCMenuFactory
{
public:
    ObjectsCMenuFactory();

    ContextMenu* create();
};

#endif // OBJECTSCMENUFACTORY_H
