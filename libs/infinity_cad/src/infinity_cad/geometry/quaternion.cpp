#include <infinity_cad/geometry/quaternion.h>
#include <stdexcept>
#include <infinity_cad/math/math.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Quaternion::Quaternion(float q0, float q1, float q2, float q3){
    entries.push_back(q0);
    entries.push_back(q1);
    entries.push_back(q2);
    entries.push_back(q3);
}

Quaternion::Quaternion(const vec3& vec, float degree){
    float rad = ifc::angleToRadians(degree);

    float sinValue = sin(rad/2);

    float q0 = cos(rad/2);
    float q1 = vec[0] * sinValue;
    float q2 = vec[1] * sinValue;
    float q3 = vec[2] * sinValue;

    entries.push_back(q0);
    entries.push_back(q1);
    entries.push_back(q2);
    entries.push_back(q3);
}

Quaternion::Quaternion(const vec3& vec){
    float q0 = 0;
    float q1 = vec[0];
    float q2 = vec[1];
    float q3 = vec[2];

    entries.push_back(q0);
    entries.push_back(q1);
    entries.push_back(q2);
    entries.push_back(q3);
}


Quaternion::~Quaternion(){

}

//-----------------------//
//  PRIVATE
//-----------------------//

float Quaternion::norm(){
    const Quaternion& Q = *this;

    float norm = sqrt((Q[0] * Q[0]) + (Q[1] * Q[1]) +
                              (Q[2] * Q[2]) + (Q[3] * Q[3]));

    return norm;
}

//-----------------------//
//  PUBLIC 
//-----------------------//

Quaternion Quaternion::conjugate(){
    const Quaternion& Q = *this;

    float q0 = Q[0];
    float q1 = -Q[1];
    float q2 = -Q[2];
    float q3 = -Q[3];

    return Quaternion(q0, q1, q2, q3);
}

void Quaternion::normalize(){
    float normValue = norm();

    this->entries[0] /= normValue;
    this->entries[1] /= normValue;
    this->entries[2] /= normValue;
    this->entries[3] /= normValue;
}

vec3 Quaternion::rotate(const vec3& vec, const vec3& axis,
                        float degree){
    Quaternion Qv(vec);

    Quaternion Qa(axis, degree);
    Quaternion QaConj = Qa.conjugate();

    Quaternion rotatedQuat = Qa * Qv * QaConj;
/*
    if(rotatedQuat[0] != 0)
        throw std::invalid_argument("Quaternion Rotate - Scalar part non zero");
*/
    return vec3(rotatedQuat[1], rotatedQuat[2], rotatedQuat[3]);
}

//-----------------------//
//  OPERATORS
//-----------------------//

Quaternion Quaternion::operator*(const Quaternion&Q2) const{
    float q0, q1, q2, q3;
    const Quaternion& Q1 = *this;
    q0 = (Q1[0] * Q2[0]) - (Q1[1] * Q2[1]) -
         (Q1[2] * Q2[2]) - (Q1[3] * Q2[3]);
    q1 = (Q1[0] * Q2[1]) + (Q1[1] * Q2[0]) +
         (Q1[2] * Q2[3]) - (Q1[3] * Q2[2]);
    q2 = (Q1[0] * Q2[2]) - (Q1[1] * Q2[3]) +
         (Q1[2] * Q2[0]) + (Q1[3] * Q2[1]);
    q3 = (Q1[0] * Q2[3]) + (Q1[1] * Q2[2]) -
         (Q1[2] * Q2[1]) + (Q1[3] * Q2[0]);

    Quaternion newQ(q0, q1, q2, q3);
    return newQ;
}

float& Quaternion::operator[](int i){
    if(i < 0 || i > 3)
        throw std::invalid_argument("Quaternion - index out of bounds");
    return entries[i];

}
const float& Quaternion::operator[](int i) const{
    if(i < 0 || i > 3)
        throw std::invalid_argument("Quaternion - index out of bounds");
    return entries[i];
}

//-----------------------//
//  PRIVATE 
//-----------------------//