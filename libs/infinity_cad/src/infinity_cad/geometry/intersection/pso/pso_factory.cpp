//
// Created by jakub on 6/13/16.
//


#include <infinity_cad/geometry/intersection/pso/pso_factory.h>
#include <infinity_cad/geometry/intersection/pso/intersection_decoder.h>
#include <pso/neighbourhood_updaters/nb_star_topology.h>
#include <pso/particle_updaters/particle_updater_spherical.h>
#include <infinity_cad/geometry/intersection/pso/intersection_fitness.h>

using namespace pso;

PSOFactory::PSOFactory(int swarmSize, double maxVelocity,
                       int maximumIterations, int threadCount,
                       Surface* surface1, Surface* surface2){
    this->surface1 = surface1;
    this->surface2 = surface2;

    this->swarmSize = swarmSize;

    this->maxVelocity = maxVelocity;

    this->maximumIterations = maximumIterations;
    this->threadCount = threadCount;

    particleDimension = 4;

    posIntervalMin = 0.0f;
    posIntervalMax = 1.0f;

    velIntervalMin = -0.5f;
    velIntervalMax = 0.5f;

    learningFactor = 0.5 + log(2.0);
    particleVelocityWeight = 1.0 / (2.0 * log(2.0));
}

PSOFactory::~PSOFactory() {

}

//-----------------------//
//  PRIVATE
//-----------------------//

ParticleFactory PSOFactory::createParticleFactory(int swarmSize,
                                                  int particleDimension,
                                                  double posIntervalMin,
                                                  double posIntervalMax,
                                                  double velIntervalMin,
                                                  double velIntervalMax,
                                                  double maxVelocity ){

    ParticleFactory particleFactory(
            swarmSize, particleDimension,
            posIntervalMin, posIntervalMax,
            velIntervalMin, velIntervalMax,
            maxVelocity
    );

    return particleFactory;
}

ParticleDecoder* PSOFactory::createParticleDecoder(){
    ParticleDecoder * particleDecoder = new IntersectionDecoder();

    return particleDecoder;
}

FitnessUpdater* PSOFactory::createFitnessUpdater(
        ParticleShPtr_ConstVectorShPtr particles,
        ParticleDecoder * particleDecoder){

    FitnessUpdater* fitnessUpdater
            = new IntersectionFitness(particles, particleDecoder,
                                      surface1, surface2);

    return fitnessUpdater;
}

NeighbourhoodUpdater* PSOFactory::createNeighbourhoodUpdater(
        ParticleShPtr_ConstVectorShPtr particles){
    int numberOfParticlesToInform = 3;
    NeighbourhoodUpdater* neighbourhoodUpdater = new NBStarTopology(
            particles, numberOfParticlesToInform
    );

    return neighbourhoodUpdater;
}


ParticleUpdater* PSOFactory::createPaticleUpdater(
        ParticleShPtr_ConstVectorShPtr particles,
        double learningFactor, double velocityWeight){
    int timesToAttemptPointGenerationWithinHyperSphere = 1;
    ParticleUpdater* particleUpdater = new ParticleUpdaterSpherical(
            particles,
            learningFactor,velocityWeight,
            timesToAttemptPointGenerationWithinHyperSphere
    );

    return particleUpdater;
}

//-----------------------//
//  PUBLIC
//-----------------------//


pso::PSO *PSOFactory::createPSO() {

    ParticleFactory particleFactory
            = createParticleFactory(swarmSize, particleDimension,
                                    posIntervalMin, posIntervalMax,
                                    velIntervalMin, velIntervalMax,
                                    maxVelocity );

    ParticleShPtr_ConstVectorShPtr particles =
            particleFactory.createUnifromParticles();

    ParticleDecoder* particleDecoder = createParticleDecoder();
    FitnessUpdater* fitnessUpdater
            = createFitnessUpdater(particles, particleDecoder);

    NeighbourhoodUpdater* neighbourhoodUpdater
            = createNeighbourhoodUpdater(particles);


    ParticleUpdater* particleUpdater
            = createPaticleUpdater(particles, learningFactor,
                                   particleVelocityWeight);

    PSO * pso = new PSO(particles,
                        fitnessUpdater,
                        neighbourhoodUpdater,
                        particleUpdater,
                        maximumIterations, threadCount);

    return pso;
}

pso::PSO *PSOFactory::createPSO(Point<double> startPos) {

    ParticleFactory particleFactory
            = createParticleFactory(swarmSize, particleDimension,
                                    posIntervalMin, posIntervalMax,
                                    velIntervalMin, velIntervalMax,
                                    maxVelocity );

    ParticleShPtr_ConstVectorShPtr particles =
            particleFactory.createUnifromParticles(startPos);

    ParticleDecoder* particleDecoder = createParticleDecoder();
    FitnessUpdater* fitnessUpdater
            = createFitnessUpdater(particles, particleDecoder);

    NeighbourhoodUpdater* neighbourhoodUpdater
            = createNeighbourhoodUpdater(particles);


    ParticleUpdater* particleUpdater
            = createPaticleUpdater(particles, learningFactor,
                                   particleVelocityWeight);

    PSO * pso = new PSO(particles,
                        fitnessUpdater,
                        neighbourhoodUpdater,
                        particleUpdater,
                        maximumIterations, threadCount);

    return pso;
}
