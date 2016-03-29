//
// Created by jakub on 12/29/15.
//

#include <pso/particle_updater.h>

using namespace pso;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

ParticleUpdater::ParticleUpdater(
                ParticleShPtr_ConstVectorShPtr particles) {
    this->particles = particles;
}

ParticleUpdater::~ParticleUpdater() {

}

//-----------------------------------------------------------//
//  PROTECTED METHODS
//-----------------------------------------------------------//

void ParticleUpdater::boundParticleWithinSolutionSpace(Particle& p) {
    const Point<double>& position = p.getPosition();

    const double intervalMin = p.getPosIntervalMin();
    const double intervalMax = p.getPosIntervalMax();

    // For each dimension check if the position is out of
    // solution space. If yes put it back on the edge
    // and reset the velocity
    for(int i = 0; i < position.size(); i++) {
        if(position[i] < intervalMin) {
            p.setPositionDim(intervalMin, i);
            p.setVelocityDim(0, i);
        }

        if(position[i] > intervalMax) {
            p.setPositionDim(intervalMax, i);
            p.setVelocityDim(0, i);
        }
    }
}
