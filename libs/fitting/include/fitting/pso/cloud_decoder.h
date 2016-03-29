//
// Created by jakub on 3/29/16.
//

#ifndef MG1_CLOUD_DECODER_H
#define MG1_CLOUD_DECODER_H

#include <pso/particle_decoder.h>

class CloudDecoder : public pso::ParticleDecoder {
private:
    pso::PSOObject* decodePosition(const pso::PointDouble position) const;

public:

    CloudDecoder();

    ~CloudDecoder();

    virtual pso::PSOObject *decodeCurrent(const pso::Particle &p) const
            override;

    virtual pso::PSOObject *decodePBest(const pso::Particle &p) const override;
};


#endif //MG1_CLOUD_DECODER_H
