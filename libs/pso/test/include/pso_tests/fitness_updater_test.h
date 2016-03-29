//
// Created by jakub on 1/6/16.
//

#ifndef BACHELOR_THESIS_FITNESS_UPDATER_TEST_H
#define BACHELOR_THESIS_FITNESS_UPDATER_TEST_H

#include <pso/fitness_updater.h>
#include <pso/entities/particle.h>
#include <pso/particle_decoder.h>

using namespace pso;

class FitnessUpdaterTest : public FitnessUpdater{
protected:
    virtual double fitnessValue(const Particle& p) override{

    }
public:

    FitnessUpdaterTest(ParticleShPtr_ConstVectorShPtr particles,
                       ParticleDecoder* particleDecoder) :
            FitnessUpdater(particles, particleDecoder){

    }
};


#endif //BACHELOR_THESIS_FITNESS_UPDATER_TEST_H
