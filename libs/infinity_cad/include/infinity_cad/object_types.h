//
// Created by jakub on 5/18/16.
//

#ifndef IC_OBJECT_TYPES_H
#define IC_OBJECT_TYPES_H

#include <string>

extern const std::string OBJ_NAME_UNKNOWN;

extern const std::string OBJ_NAME_POINT;

extern const std::string OBJ_NAME_CURVE_BEZIER;
extern const std::string OBJ_NAME_CURVE_BSPLINE;
extern const std::string OBJ_NAME_CURVE_INTERP;

extern const std::string OBJ_NAME_SURFACE_BEZIER;
extern const std::string OBJ_NAME_SURFACE_BSPLINE;
extern const std::string OBJ_NAME_SURFACE_BEZIER_RECT;
extern const std::string OBJ_NAME_SURFACE_BEZIER_CYLIND;
extern const std::string OBJ_NAME_SURFACE_BSPLINE_RECT;
extern const std::string OBJ_NAME_SURFACE_BSPLINE_CYLIND;


class ObjectType{
private:
    std::string name;
public:
    ObjectType(){
        this->name = "";
    }

    ObjectType(std::string name){
        this->name = name;
    }

    std::string toString() const {
        if(name == OBJ_NAME_SURFACE_BEZIER_RECT ||
                name == OBJ_NAME_SURFACE_BEZIER_CYLIND){
            return OBJ_NAME_SURFACE_BEZIER;
        }else if(name == OBJ_NAME_SURFACE_BSPLINE_RECT ||
                 name == OBJ_NAME_SURFACE_BSPLINE_CYLIND){
            return OBJ_NAME_SURFACE_BSPLINE;
        }else{
            return name;
        }
    }

    bool isSurface() const{
        return (name == OBJ_NAME_SURFACE_BEZIER_CYLIND ||
                name == OBJ_NAME_SURFACE_BEZIER_RECT ||
                name == OBJ_NAME_SURFACE_BSPLINE_RECT ||
                name == OBJ_NAME_SURFACE_BSPLINE_CYLIND);
    }

    bool operator==(const ObjectType& t) const{
        return (t.name == this->name);
    }
    bool operator!=(const ObjectType& t) const{
        return (t.name != this->name);
    }
};


extern const ObjectType OBJ_TYPE_UNKNOWN;

extern const ObjectType OBJ_TYPE_POINT;

extern const ObjectType OBJ_TYPE_CURVE_BEZIER;
extern const ObjectType OBJ_TYPE_CURVE_BSPLINE;
extern const ObjectType OBJ_TYPE_CURVE_INTERP;

extern const ObjectType OBJ_TYPE_SURFACE_BEZIER_RECT;
extern const ObjectType OBJ_TYPE_SURFACE_BEZIER_CYLIND;
extern const ObjectType OBJ_TYPE_SURFACE_BSPLINE_RECT;
extern const ObjectType OBJ_TYPE_SURFACE_BSPLINE_CYLIND;

#endif //IC_OBJECT_TYPES_H
