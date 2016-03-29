//
// Created by jakub on 3/29/16.
//

#include <fitting/pso/transform_vector.h>

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

TransformVector::TransformVector(){
    x = y = z = xAngle = yAngle = zAngle = 0;
}

TransformVector::TransformVector(float x, float y, float z,
                                 float xAngle, float yAngle, float zAngle) :
    x(x), y(y), z(z), xAngle(xAngle), yAngle(yAngle), zAngle(zAngle){

}

TransformVector::~TransformVector() {

}
