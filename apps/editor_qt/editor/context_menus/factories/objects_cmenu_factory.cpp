#include "objects_cmenu_factory.h"

#include "context_menus/context_menu_settings.h"

ObjectsCMenuFactory::ObjectsCMenuFactory()
{

}

ContextMenu* ObjectsCMenuFactory::create(){
    ContextMenu* objectListContextMenu = new ContextMenu();

    auto func = [](std::string){ };

    objectListContextMenu->addAction((OBJECTS_MENU_ADD_NAME),
                                    OBJECTS_MENU_ADD_HANDLER);
    objectListContextMenu->addAction((OBJECTS_MENU_ADD_NAMED_NAME),
                                    OBJECTS_MENU_ADD_NAMED_HANDLER);
    objectListContextMenu->addAction((OBJECTS_MENU_PROPERTIES_NAME),
                                    Handler(func));

    return objectListContextMenu;
}
