//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_PARTICLE_DECODER_T_H
#define BACHELOR_THESIS_PARTICLE_DECODER_T_H

#include <pso/pso_common.h>

namespace pso {
class ParticleDecoder {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTOR
    //-----------------------------------------------------------//

    ParticleDecoder();

    virtual ~ParticleDecoder();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    /*
     * Decodes particle's current position
     */
    virtual PSOObject* decodeCurrent(const Particle & p) const = 0;

    /*
     * Decodes particles's personal best position
     */
    virtual PSOObject* decodePBest(const Particle & p) const = 0;
};
}

#endif //BACHELOR_THESIS_PARTICLE_DECODER_T_H
