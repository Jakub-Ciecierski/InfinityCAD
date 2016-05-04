//
// Created by jakub on 5/2/16.
//

#include <infinity_cad/geometry/surfaces/bicubic_bezier_patch.h>
#include <stdexcept>

using namespace ifc;
using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BicubicBezierPatch::BicubicBezierPatch(Matrix<ifc::Point*> points) :
        points(points){
    update();
}

BicubicBezierPatch::~BicubicBezierPatch(){

}

//-----------------------//
//  PUBLIC 
//-----------------------//

const mat4& BicubicBezierPatch::getX() const{
    return x;
}

const mat4& BicubicBezierPatch::getY() const{
    return y;
}

const mat4& BicubicBezierPatch::getZ() const{
    return z;
}

const Matrix<ifc::Point*>& BicubicBezierPatch::getPoints(){
    return points;
}
/*
ifc::Point** BicubicBezierPatch::getColumn(int i){
    if(i < 0 || i > CUBIC_COUNT)
        throw std::invalid_argument("BicubicBezierPatch::getColumn() - Index "
                                            "out of bound");
    return points[i];
}
ifc::Point** BicubicBezierPatch::getRow(int i){
    if(i < 0 || i > CUBIC_COUNT)
        throw std::invalid_argument("BicubicBezierPatch::getRow() - Index "
                                            "out of bound");
    ifc::Point** row = new ifc::Point*[CUBIC_COUNT];
    for(int j = 0; j < CUBIC_COUNT; j++){
        row[j] = points[j][i];
    }

    return row;
}*/

void BicubicBezierPatch::update(){
    for(int i = 0; i < CUBIC_COUNT; i ++){
        for(int j = 0; j < CUBIC_COUNT; j ++){
            const vec3& pos = points[i][j]->getPosition();
            x[i][j] = pos.x;
            y[i][j] = pos.y;
            z[i][j] = pos.z;
        }
    }
}

//-----------------------//
//  PRIVATE 
//-----------------------//
