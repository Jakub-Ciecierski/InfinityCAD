//
// Created by jakub on 12/29/15.
//

#include <pso/particle_factory.h>
#include <pso/entities/particle.h>
#include <sstream>
#include <math/random.h>

using namespace pso;

ParticleFactory::ParticleFactory(int swarmSize, int particleDimension,
                                          double posIntervalMin,
                                          double posIntervalMax,
                                          double velIntervalMin,
                                          double velIntervalMax,
                                          double maxVelocity) :
                swarmSize(swarmSize), particleDimension(particleDimension),
                posIntervalMin(posIntervalMin), posIntervalMax(posIntervalMax),
                velIntervalMin(velIntervalMin), velIntervalMax(velIntervalMax),
                maxVelocity(maxVelocity) {

}

ParticleFactory::~ParticleFactory() {

}


ParticleShPtr_ConstVectorShPtr ParticleFactory::createUnifromParticles() {

    ParticleShPtr_Vector* particles = new ParticleShPtr_Vector(swarmSize);

    for(unsigned int i = 0; i < swarmSize; i++){
        Point<double> position(particleDimension);
        Point<double> velocity(particleDimension);

        for(unsigned int j = 0; j < particleDimension; j++){
            position[j] = rnd::generateRandomNumber(posIntervalMin,
                                                      posIntervalMax);
            velocity[j] = rnd::generateRandomNumber(velIntervalMin,
                                                      velIntervalMax);
        }

        ParticleShPtr particle = std::make_shared<Particle>(
                posIntervalMin, posIntervalMax,
                velIntervalMin, velIntervalMax,
                maxVelocity,
                position, velocity
        );
        (*particles)[i] = (particle);
    }

    ParticleShPtr_ConstVectorShPtr particlesShPtr(particles);

    return particlesShPtr;
}

ParticleShPtr_ConstVectorShPtr ParticleFactory::createUnifromParticles(Point<double> position) {

    ParticleShPtr_Vector* particles = new ParticleShPtr_Vector(swarmSize);

    for(unsigned int i = 0; i < swarmSize; i++){
        Point<double> velocity(particleDimension);

        for(unsigned int j = 0; j < particleDimension; j++){
            velocity[j] = rnd::generateRandomNumber(velIntervalMin,
                                                    velIntervalMax);
        }

        ParticleShPtr particle = std::make_shared<Particle>(
                posIntervalMin, posIntervalMax,
                velIntervalMin, velIntervalMax,
                maxVelocity,
                position, velocity
        );
        (*particles)[i] = (particle);
    }

    ParticleShPtr_ConstVectorShPtr particlesShPtr(particles);

    return particlesShPtr;
}