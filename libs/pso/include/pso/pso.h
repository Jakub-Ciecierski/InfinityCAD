//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_PSO_T_H
#define BACHELOR_THESIS_PSO_T_H

#include <pso/entities/particle.h>
#include <pso/pso_common.h>

#include <threading/thread_pool.h>
#include <pso/console/info_printer.h>

namespace pso{

    extern void* startParallelPSO(void* argv);

/*
 * Encapsulates all the parts of PSO algorithm.
 *
 * Has ownership over:
 *      - Raw pointer to
 *          - FitnessUpdater
 *          - NeighbourhoodUpdater
 *          - ParticleUpdater
 */
class PSO {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    ParticleShPtr_ConstVectorShPtr particles;

    FitnessUpdater* fitnessUpdater;
    NeighbourhoodUpdater* neighUpdater;
    ParticleUpdater* particleUpdater;

    ParticleShPtr currentGenerationBestParticle;

    Particle globalBestParticle;

    int maxIterations;

    unsigned int swarmSize;

    unsigned int threadCount;

    InfoPrinter* infoPrinter;

    const int noFitnessUpdateInARowCountBeforeReset = 99999;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    void initThreadVariables(thread_argv *targs);

    void compute(thread_argv *targs);

    void computeIteration(thread_argv *targs);

    void computeFitness(thread_argv *targs);

    void computeGlobalBest(thread_argv *targs);

    void computeNeighbourhood(thread_argv *targs);

    void computeParticleUpdate(thread_argv *targs);

    void printIterationInfo(thread_argv *targs);

    void measureTime(thread_argv *targs);

    void runEndingCriteria(thread_argv *targs);

    void resetCurrentGenerationParticles();

    /*
     * Checks whether the PSO should converge
     */
    bool hasConvereged(const int* iter);

    /*
     * Updates the best particle among all particles.
     * Returns true if new particle has became best
     */
    bool updateBestParticle();

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//;

    PSO(ParticleShPtr_ConstVectorShPtr particles,
        FitnessUpdater* fitnessUpdater,
        NeighbourhoodUpdater* neighUpdater,
        ParticleUpdater* particleUpdater,
        int maxIterations,
        unsigned int threadCount);

    ~PSO();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    /*
     * Starts the computations
     */
    void start();

    /*
     * Returns the best particle found by the computations
     */
    const Particle* getBestParticle() const;

    /*
     * Decodes best particle and returns the PSOObject
     * representing this particle
     */
    const PSOObject* getBestPSOObject() const;

    const ParticleDecoder & getDecoder() const;

    //-----------------------------------------------------------//
    //  FRIEND METHODS
    //-----------------------------------------------------------//

    friend void* startParallelPSO(void* argv);
};
}

#endif //BACHELOR_THESIS_PSO_T_H
