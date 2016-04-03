#ifndef CONTEXTMENUSETTINGS_H
#define CONTEXTMENUSETTINGS_H

#include <string>
#include "context_menus/handler.h"

extern const std::string OBJECTS_MENU_ADD_NAME;
extern const std::string OBJECTS_MENU_ADD_NAMED_NAME;
extern const std::string OBJECTS_MENU_PROPERTIES_NAME;

extern const std::string SCENE_MENU_MOVE_CROSS_NAME;
extern const std::string SCENE_MENU_MOVE_CAMERA_NAME;
extern const std::string SCENE_MENU_DELETE_NAME;
extern const std::string SCENE_MENU_CHANGE_NAME_NAME;

extern Handler OBJECTS_MENU_ADD_HANDLER;
extern Handler OBJECTS_MENU_ADD_NAMED_HANDLER;
extern Handler OBJECTS_MENU_PROPERTIES_HANDLER;

extern Handler SCENE_MENU_MOVE_CROSS_HANDLER;
extern Handler SCENE_MENU_MOVE_CAMERA_HANDLER;
extern Handler SCENE_MENU_DELETE_HANDLER;
extern Handler SCENE_MENU_CHANGE_NAME_HANDLER;


#endif // CONTEXTMENUSETTINGS_H
