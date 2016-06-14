//
// Created by jakub on 12/29/15.
//

#include <pso/fitness_updater.h>
#include <pso/particle_decoder.h>

using namespace pso;

FitnessUpdater::FitnessUpdater(ParticleShPtr_ConstVectorShPtr particles,
                                const ParticleDecoder * particleDecoder){
    this->particleDecoder = particleDecoder;
    this->particles = particles;
}

FitnessUpdater::~FitnessUpdater(){
    delete particleDecoder;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

const ParticleDecoder & FitnessUpdater::getParticleDecoder() const {
    return *particleDecoder;
}

const ParticleShPtr_Vector& FitnessUpdater::getParticles() const {
    return *(particles.get());
}

void FitnessUpdater::actOn(Particle& particle){

}

void FitnessUpdater::update(int startIndex, int finishIndex) {
    for(int i = startIndex; i <= finishIndex; i++){
        Particle& p = *((*particles)[i]);
        double fitnessValue = this->fitnessValue(p);
        p.setFitness(fitnessValue);

        if(fitnessValue < p.getBestFitness()){
            p.saveCurrentConfigAsBest();
        }
    }
}
