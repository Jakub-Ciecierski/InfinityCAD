//
// Created by jakub on 1/4/16.
//

#include <gtest/gtest.h>
#include <pso_tests/pso_test.h>

using namespace pso;

Particle createParticle_s4_r2(){
    unsigned int stateCount = 4;
    unsigned int symbolCount = 2;

    double posMin = 0.0f;
    double posMax = 5.0f;

    double velMin = 0.0f;
    double velMax = 5.0f;

    double maxVel = 1.0f;

    Point<double> pos(8);
    pos[0] = 2.0f;
    pos[1] = 3.0f;
    pos[2] = 3.0f;
    pos[3] = 4.0f;

    pos[4] = 4.0f;
    pos[5] = 3.0f;
    pos[6] = 3.0f;
    pos[7] = 4.0f;

    Point<double> vel(8);

    Particle p(posMin, posMax,
                 velMin, velMax,
                 maxVel,
                 pos, vel);
    return p;
}

PSO createPSOInstance(){
    int particleDimension = 10;

    double posIntervalMin = 0.0f;
    double posIntervalMax = 5.0f;

    double velIntervalMin = -5.0f;
    double velIntervalMax = 5.0f;

    double maxVelocity = 10.0f;

    int swarmSize = 50;

    int maxIterations = 2;

    unsigned int threadCount = 8;

    ParticleFactory particleFactory
            (
                    swarmSize, particleDimension,
                    posIntervalMin, posIntervalMax,
                    velIntervalMin, velIntervalMax,
                    maxVelocity
            );

    ParticleDecoder* particleDecoder = new ParticleDecoderTest();

    ParticleShPtr_ConstVectorShPtr particles =
            particleFactory.createUnifromParticles();

    FitnessUpdater* fitnessUpdater = new FitnessUpdaterTest
            (
                    particles, particleDecoder
            );

    NeighbourhoodUpdater* neighbourhoodUpdater = new NeighbourhoodUpdaterTest
            (
                    particles
            );
    ParticleUpdater* particleUpdater = new ParticleUpdaterTest
            (
                    particles
            );

    PSO pso(particles,
              fitnessUpdater,
              neighbourhoodUpdater,
              particleUpdater,
              maxIterations,
              threadCount);

    return pso;

}

TEST(PSO, GetBestParticleBeforeStart_Null) {
    PSO pso = createPSOInstance();

    const Particle* bestParticle = pso.getBestParticle();

    //EXPECT_EQ(NULL, bestParticle);
}

TEST(PSO, Start) {
    PSO pso = createPSOInstance();

    pso.start();

    const Particle* bestParticle = pso.getBestParticle();

    //EXPECT_EQ(NULL, bestParticle);
}