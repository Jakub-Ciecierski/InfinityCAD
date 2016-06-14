//
// Created by jakub on 6/13/16.
//

#include <pso/pso_object.h>
#include <pso/entities/particle.h>
#include <infinity_cad/geometry/intersection/pso/intersection_decoder.h>
#include <infinity_cad/geometry/intersection/pso/param_pso_object.h>

using namespace pso;

IntersectionDecoder::IntersectionDecoder(){

}

IntersectionDecoder::~IntersectionDecoder() {

}

pso::PSOObject *IntersectionDecoder::decodePosition(
        const pso::PointDouble position) const {
    glm::vec2 param1;
    glm::vec2 param2;

    param1.x = position[0];
    param1.y = position[1];

    param2.x = position[2];
    param2.y = position[3];

    ParamPSOObject* psoObject = new ParamPSOObject(param1, param2);

    return psoObject;
}

pso::PSOObject *IntersectionDecoder::decodeCurrent(
        const pso::Particle &p) const {
    const PointDouble& position = p.getPosition();

    return this->decodePosition(position);
}

pso::PSOObject *IntersectionDecoder::decodePBest(const pso::Particle &p) const {
    const PointDouble& position = p.getPbest();

    return this->decodePosition(position);
}

