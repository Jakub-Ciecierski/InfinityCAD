//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_FITNESS_UPDATER_H
#define BACHELOR_THESIS_FITNESS_UPDATER_H

#include <pso/entities/particle.h>
#include <pso/pso_common.h>

namespace pso {
/*
 * Pure Abstract class used to provide interface for Fitness update
 *
 * Has ownership over:
 *      - Raw pointer to ParticleDecoder
 *      - Shared pointer to ParticleShPtr_ConstVectorShPtr
 */
class FitnessUpdater {
protected:

    //-----------------------------------------------------------//
    //  PROTECTED FIELDS
    //-----------------------------------------------------------//

    const ParticleDecoder * particleDecoder;

    ParticleShPtr_ConstVectorShPtr particles;

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    /*
     * Computes fitness value of a single particle
     */
    virtual double fitnessValue(const Particle & p) = 0;

public:

    //-----------------------------------------------------------//
    //  CONSTRUCTOR
    //-----------------------------------------------------------//

    FitnessUpdater(ParticleShPtr_ConstVectorShPtr particles,
                   const ParticleDecoder * particleDecoder);

    virtual ~FitnessUpdater();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    const ParticleDecoder & getParticleDecoder() const;
    const ParticleShPtr_Vector& getParticles() const;

    virtual void actOn(Particle& particle);

    /*
     * Updates the particles from startIndex to finishIndex
     */
    void update(int startIndex, int finishIndex);
};
}

#endif //BACHELOR_THESIS_FITNESS_UPDATER_H
