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
        controlPoints(points){
    update();
}

BicubicBezierPatch::BicubicBezierPatch(Matrix<glm::vec4> points) :
        controlPoints(0,0){
    for(int i = 0; i < CUBIC_COUNT; i ++){
        for(int j = 0; j < CUBIC_COUNT; j ++){
            const vec4& pos = points[i][j];
            x[i][j] = pos.x;
            y[i][j] = pos.y;
            z[i][j] = pos.z;
        }
    }
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

Matrix<ifc::Point*>& BicubicBezierPatch::getPoints(){
    return controlPoints;
}

void BicubicBezierPatch::update(){
    for(int i = 0; i < CUBIC_COUNT; i ++){
        for(int j = 0; j < CUBIC_COUNT; j ++){
            ifc::Point* point = controlPoints[i][j];
            if(point == NULL){
                if(j > 1){
                    point = controlPoints[i][j-1];
                }else{
                    continue;
                }
            }
            const vec3& pos = point->getPosition();
            x[i][j] = pos.x;
            y[i][j] = pos.y;
            z[i][j] = pos.z;
        }
    }
}

//-----------------------//
//  PRIVATE 
//-----------------------//
