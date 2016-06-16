//
// Created by jakub on 6/13/16.
//

#ifndef IC_PSO_FACTORY_H
#define IC_PSO_FACTORY_H

#include <pso/pso.h>
#include <pso/particle_factory.h>
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

class PSOFactory {
private:
    Surface* surface1;
    Surface* surface2;

    int swarmSize;
    double maxVelocity;

    int particleDimension;

    double posIntervalMin;
    double posIntervalMax;

    double velIntervalMin;
    double velIntervalMax;

    double learningFactor;
    double particleVelocityWeight;

    int maximumIterations;
    int threadCount;

    pso::ParticleFactory createParticleFactory(int swarmSize,
                                               int particleDimension,
                                               double posIntervalMin,
                                               double posIntervalMax,
                                               double velIntervalMin,
                                               double velIntervalMax,
                                               double maxVelocity );

    pso::ParticleDecoder* createParticleDecoder();

    pso::FitnessUpdater* createFitnessUpdater(
            pso::ParticleShPtr_ConstVectorShPtr particles,
            pso::ParticleDecoder * particleDecoder);

    pso::NeighbourhoodUpdater* createNeighbourhoodUpdater(
            pso::ParticleShPtr_ConstVectorShPtr particles);

    pso::ParticleUpdater* createPaticleUpdater(
            pso::ParticleShPtr_ConstVectorShPtr particles,
            double learningFactor, double velocityWeight);
public:

    PSOFactory(int swarmSize, double maxVelocity,
               int maximumIterations, int threadCount,
               Surface* surface1, Surface* surface2);

    ~PSOFactory();

    pso::PSO* createPSO();
    pso::PSO* createPSO(Point<double> startPos);
};


#endif //IC_PSO_FACTORY_H

