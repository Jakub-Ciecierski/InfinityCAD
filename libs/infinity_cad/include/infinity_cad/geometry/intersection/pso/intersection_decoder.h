//
// Created by jakub on 6/13/16.
//

#ifndef IC_INTERSECTION_DECODER_H
#define IC_INTERSECTION_DECODER_H

#include <pso/pso_common.h>
#include <pso/particle_decoder.h>

class IntersectionDecoder : public pso::ParticleDecoder {
private:
    pso::PSOObject* decodePosition(const pso::PointDouble position) const;
public:

    IntersectionDecoder();

    ~IntersectionDecoder();

    virtual pso::PSOObject *decodeCurrent(const pso::Particle &p) const
            override;

    virtual pso::PSOObject *decodePBest(const pso::Particle &p) const override;
};


#endif //IC_INTERSECTION_DECODER_H
