//
// Created by jakub on 3/29/16.
//

#include <pso/pso_object.h>
#include <pso/entities/particle.h>
#include <fitting/pso/cloud_decoder.h>
#include <fitting/pso/transform_vector.h>

using namespace pso;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

CloudDecoder::CloudDecoder() {

}

CloudDecoder::~CloudDecoder() {

}

//-----------------------//
//  PRIVATE
//-----------------------//

PSOObject* CloudDecoder::decodePosition(const PointDouble position) const{
    TransformVector* tv = new TransformVector();

    tv->x = position[0];
    tv->y = position[1];
    tv->z = position[2];

    tv->xAngle = position[3];
    tv->yAngle = position[4];
    tv->zAngle = position[5];

    return tv;
}

//-----------------------//
//  PUBLIC 
//-----------------------//


PSOObject* CloudDecoder::decodeCurrent(const Particle & p) const{
    const PointDouble& position = p.getPosition();

    return this->decodePosition(position);
}

pso::PSOObject *CloudDecoder::decodePBest(const Particle &p) const {
    const PointDouble& position = p.getPosition();

    return this->decodePosition(position);
}

