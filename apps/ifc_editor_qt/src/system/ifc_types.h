#ifndef OBJECTS_SETTINGS_H
#define OBJECTS_SETTINGS_H

#include <string>

/*
 * Settings for objects names.
 * All Objects from IC library should be defined here for
 * viewing in object list
 * */

struct Type{
    std::string type;
    Type(){
        type = "";
    }
    Type(std::string type){
        this->type = type;
    }

    bool operator==(const Type& t) const{
        return (t.type == this->type);
    }
    bool operator!=(const Type& t) const{
        return (t.type != this->type);
    }
};

extern const std::string OBJ_ROOT_NAME;
extern const std::string OBJ_CAMERA_NAME;

extern const std::string RB_ROOT_NAME;
extern const std::string RB_TORUS_NAME;
extern const std::string RB_POINT_NAME;
extern const std::string RB_BEZIER_NAME;
extern const std::string RB_BSPLINE_NAME;
extern const std::string RB_BSPLINE_INTERPOLATING_NAME;
extern const std::string RB_POINT_BEZIER_NAME;
extern const std::string RB_POINT_CLONE_NAME;

extern const std::string RB_INTERSECTION_CURVE_NAME;

extern const std::string RB_SURFACE_CO_RECT_NAME;
extern const std::string RB_SURFACE_CO_CYLIND_NAME;

extern const std::string RB_SURFACE_C2_RECT_NAME;
extern const std::string RB_SURFACE_C2_CYLIND_NAME;
extern const std::string RB_SURFACE_GREGORY_NAME;

extern const Type RB_ROOT_TYPE;
extern const Type RB_TORUS_TYPE;
extern const Type RB_POINT_TYPE;
extern const Type RB_BEZIER_TYPE;
extern const Type RB_BSPLINE_TYPE;
extern const Type RB_BSPLINE_INTERPOLATING_TYPE;
extern const Type RB_POINT_BEZIER_TYPE;
extern const Type RB_POINT_CLONE_TYPE;

extern const Type RB_INTERSECTION_CURVE_TYPE;

extern const Type RB_SURFACE_C0_RECT_TYPE;
extern const Type RB_SURFACE_C0_CYLIND_TYPE;

extern const Type RB_SURFACE_C2_RECT_TYPE;
extern const Type RB_SURFACE_C2_CYLIND_TYPE;
extern const Type RB_SURFACE_GREGORY_TYPE;

extern const Type OBJ_ROOT_TYPE;
extern const Type OBJ_CAMERA_TYPE;

Type typeFromString(std::string);

bool isSurface(const Type& type);
bool canAddChildren(const Type& type);

#endif // OBJECTS_SETTINGS_H
