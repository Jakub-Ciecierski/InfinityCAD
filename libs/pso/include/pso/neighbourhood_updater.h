//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_NEIGHBOURHOOD_UPDATER_H
#define BACHELOR_THESIS_NEIGHBOURHOOD_UPDATER_H

#include <pso/pso_common.h>

namespace pso {
/*
 * Pure Abstract class used to provide interface for Neighbourhood update.
 *
 * Classes deriving from this base should implement the update function
 * in order to update the local best position of each particle.
 *
 * Has ownership over:
 *      - Shared pointer to ParticleShPtr_ConstVectorShPtr
 */
class NeighbourhoodUpdater {
protected:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    ParticleShPtr_ConstVectorShPtr particles;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTOR
    //-----------------------------------------------------------//

    NeighbourhoodUpdater(ParticleShPtr_ConstVectorShPtr particles);

    virtual ~NeighbourhoodUpdater();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual void update() = 0;
};
}

#endif //BACHELOR_THESIS_NEIGHBOURHOOD_UPDATER_H
