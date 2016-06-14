//
// Created by jakub on 12/29/15.
//

#include <pso/neighbourhood_updater.h>

//-----------------------------------------------------------//
//  CONSTRUCTOR
//-----------------------------------------------------------//

using namespace pso;

NeighbourhoodUpdater::NeighbourhoodUpdater(
                        ParticleShPtr_ConstVectorShPtr particles){
    this->particles = particles;
}

NeighbourhoodUpdater::~NeighbourhoodUpdater() {

}
