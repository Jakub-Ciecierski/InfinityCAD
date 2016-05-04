#include "system/ifc_types.h"
#include <stdexcept>

using namespace std;

const string RB_ROOT_NAME = "Rigid Bodies";
const string RB_TORUS_NAME = "Torus";
const string RB_POINT_NAME = "Point";
const string RB_BEZIER_NAME = "Bezier Curve";
const string RB_BSPLINE_NAME = "B-Spline";
const string RB_BSPLINE_INTERPOLATING_NAME = "B-Spline Interpolating";
const string RB_POINT_BEZIER_NAME = "Point Bezier";
const string RB_POINT_CLONE_NAME = "Point Clone";

const std::string RB_SURFACE_CO_RECT_NAME = "Surface C0";

const string OBJ_ROOT_NAME = "Objects";
const string OBJ_CAMERA_NAME = "Camera";

const Type RB_ROOT_TYPE(RB_ROOT_NAME);
const Type RB_TORUS_TYPE(RB_TORUS_NAME);
const Type RB_POINT_TYPE(RB_POINT_NAME);
const Type RB_BEZIER_TYPE(RB_BEZIER_NAME);
const Type RB_BSPLINE_TYPE(RB_BSPLINE_NAME);
const Type RB_BSPLINE_INTERPOLATING_TYPE(RB_BSPLINE_INTERPOLATING_NAME);
const Type RB_POINT_BEZIER_TYPE(RB_POINT_BEZIER_NAME);
const Type RB_POINT_CLONE_TYPE(RB_POINT_CLONE_NAME);

const Type RB_SURFACE_C0_RECT_TYPE(RB_SURFACE_CO_RECT_NAME);

const Type OBJ_ROOT_TYPE(OBJ_ROOT_NAME);
const Type OBJ_CAMERA_TYPE(OBJ_CAMERA_NAME);

Type typeFromString(std::string typeStr){
    if(typeStr == RB_ROOT_NAME)
        return RB_ROOT_TYPE;
    if(typeStr == RB_TORUS_NAME)
        return RB_TORUS_TYPE;
    if(typeStr == RB_POINT_NAME)
        return RB_POINT_TYPE;
    if(typeStr == RB_BEZIER_NAME)
        return RB_BEZIER_TYPE;
    if(typeStr == RB_BSPLINE_NAME)
        return RB_BSPLINE_TYPE;
    if(typeStr == RB_BSPLINE_INTERPOLATING_NAME)
        return RB_BSPLINE_INTERPOLATING_TYPE;

    if(typeStr == RB_POINT_CLONE_NAME)
        return RB_POINT_CLONE_TYPE;

    if(typeStr == OBJ_ROOT_NAME)
        return OBJ_ROOT_TYPE;
    if(typeStr == OBJ_CAMERA_NAME)
        return OBJ_CAMERA_TYPE;

    throw new std::invalid_argument("No such Type: " + typeStr);
}

bool isSurface(const Type& type){
    return (type == RB_SURFACE_C0_RECT_TYPE);
}

bool canAddChildren(const Type& type){
    return  (type == RB_BEZIER_TYPE ||
             type == RB_BSPLINE_TYPE ||
             type == RB_BSPLINE_INTERPOLATING_TYPE);
}
