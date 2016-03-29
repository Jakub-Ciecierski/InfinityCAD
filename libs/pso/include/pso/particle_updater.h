//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_PARTICLE_UPDATER_H
#define BACHELOR_THESIS_PARTICLE_UPDATER_H

#include <pso/pso_common.h>
#include <pso/entities/particle.h>

namespace pso {

/*
 * Pure Abstract class used to provide interface for Particle Update.
 *
 * Has ownership over:
 *      - Shared pointer to ParticleShPtr_ConstVectorShPtr
 */
class ParticleUpdater {
protected:
    //-----------------------------------------------------------//
    //  PROTECTED FIELDS
    //-----------------------------------------------------------//

    ParticleShPtr_ConstVectorShPtr particles;

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    /*
     * Bounds the particle within the solution space by
     * making sure that particle's position is within
     * the interval min max and resets the velocity to 0
     * in the dimension that had to be bound.
     */
    void boundParticleWithinSolutionSpace(Particle & p);

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    ParticleUpdater(ParticleShPtr_ConstVectorShPtr particles);

    virtual ~ParticleUpdater();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    /*
     * Updates the particles from startIndex to finishIndex
     */
    virtual void update(int startIndex, int finishIndex) = 0;
};
}

#endif //BACHELOR_THESIS_PARTICLE_UPDATER_H
