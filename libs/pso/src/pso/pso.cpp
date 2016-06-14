//
// Created by jakub on 12/29/15.
//

#include <pso/pso.h>
#include <pso/fitness_updater.h>
#include <pso/neighbourhood_updater.h>
#include <pso/particle_updater.h>
#include <pso/particle_decoder.h>

#include <math/random.h>
#include <clock.h>

using namespace std;

namespace pso {

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

PSO::PSO(ParticleShPtr_ConstVectorShPtr particles,
                FitnessUpdater* fitnessUpdater,
                NeighbourhoodUpdater* neighUpdater,
                ParticleUpdater* particleUpdater,
                int maxIterations,
                unsigned int threadCount):
        fitnessUpdater(fitnessUpdater),
        neighUpdater(neighUpdater),
        particleUpdater(particleUpdater),
        maxIterations(maxIterations),
        threadCount(threadCount),
        globalBestParticle(*((*particles)[0])){
    this->particles = particles;
    swarmSize = particles->size();

    infoPrinter = new InfoPrinter();
}

PSO::~PSO() {
    delete fitnessUpdater;
    delete neighUpdater;
    delete particleUpdater;
    delete infoPrinter;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void PSO::start() {
    threading::ThreadPool threadPool(threadCount);

    // Create structure for thread arguments
    thread_argv* targs = new thread_argv[threadCount];

    // Pointer to initial function
    void* (*fun_ptr)(void*);
    fun_ptr = &startParallelPSO;

    int curr_iter = 0;
    bool do_work = true;

    int reminder = this->swarmSize % threadPool.size();
    int sub_count = this->swarmSize / threadPool.size();

    // Initialize the structure with data
    for(unsigned int i = 0; i < threadCount; i++){

        int start = i * sub_count;
        int finish = start + sub_count - 1;
        if (i == threadCount - 1)
            finish += reminder;

        targs[i].startIndex = start;
        targs[i].finishIndex = finish;

        targs[i].tid = i;
        targs[i].pso = this;
        targs[i].curr_iter = &curr_iter;
        targs[i].do_work = &do_work;

        targs[i].threadPool = &threadPool;

        threadPool.createThread(fun_ptr, (&targs[i]));
    }

    threadPool.joinAll();
}

const Particle *PSO::getBestParticle() const{
    return &globalBestParticle;
}

const PSOObject* PSO::getBestPSOObject() const {
    PSOObject* psoObject =
            this->getDecoder().decodePBest((*this->getBestParticle()));

    return psoObject;
}

const ParticleDecoder &PSO::getDecoder() const {
    return this->fitnessUpdater->getParticleDecoder();
}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

void PSO::initThreadVariables(thread_argv *targs){
    if (targs->tid == threading::T_BOSS) {
        *(targs->do_work) = true;
        *(targs->curr_iter) = 0;
        targs->noFitnessUpdateInARowCount = 0;
        targs->noFitnessUpdateInARowCountBeforeReset =
                this->noFitnessUpdateInARowCountBeforeReset;
        targs->newBestParticleUpdated = false;

        targs->timeMeasures.lastFitnessTime = 0;
        targs->timeMeasures.lastNeighbourTime = 0;
        targs->timeMeasures.lastParticleUpdateTime = 0;
        targs->timeMeasures.overallTime = 0;
        targs->timeMeasures.ETA = 0;
    }
}

void PSO::compute(thread_argv *targs) {
    targs->threadPool->synchronizeAll();

    initThreadVariables(targs);

    while(*(targs->do_work)){
        printIterationInfo(targs);

        computeIteration(targs);

        runEndingCriteria(targs);

        targs->threadPool->synchronizeAll();
    }
}

void PSO::computeIteration(thread_argv *targs){
    computeFitness(targs);

    computeGlobalBest(targs);

    computeNeighbourhood(targs);

    computeParticleUpdate(targs);
}


void PSO::computeFitness(thread_argv *targs){
    targs->threadPool->synchronizeAll();

    if(targs->tid == threading::T_BOSS)
        clk::startClock();

    this->fitnessUpdater->update(targs->startIndex,
                                 targs->finishIndex);

    if(targs->tid == threading::T_BOSS)
        targs->timeMeasures.lastFitnessTime = clk::stopClock();

}

void PSO::computeGlobalBest(thread_argv *targs){
    targs->threadPool->synchronizeAll();

    if(targs->tid == threading::T_BOSS) {
        targs->newBestParticleUpdated = this->updateBestParticle();

        if(!(targs->newBestParticleUpdated))
            targs->noFitnessUpdateInARowCount++;
        else{
            targs->noFitnessUpdateInARowCount = 0;

            this->fitnessUpdater->actOn(this->globalBestParticle);
        }

    }
}

void PSO::computeNeighbourhood(thread_argv *targs){
    targs->threadPool->synchronizeAll();

    if (targs->tid == threading::T_BOSS) {
        clk::startClock();

        if(!(targs->newBestParticleUpdated))
            this->neighUpdater->update();

        targs->timeMeasures.lastNeighbourTime = clk::stopClock();
    }
}

void PSO::computeParticleUpdate(thread_argv *targs){
    targs->threadPool->synchronizeAll();

    if(targs->tid == threading::T_BOSS)
        clk::startClock();

    this->particleUpdater->update(targs->startIndex,
                                  targs->finishIndex);

    if(targs->tid == threading::T_BOSS)
        targs->timeMeasures.lastParticleUpdateTime = clk::stopClock();
}

void PSO::printIterationInfo(thread_argv *targs){
    if (targs->tid == threading::T_BOSS) {
        this->measureTime(targs);
        infoPrinter->printInfo(targs->timeMeasures,
                               *(targs->curr_iter),
                               this->maxIterations, this->swarmSize,
                               this->currentGenerationBestParticle.get(),
                               &(this->globalBestParticle));
    }
}

void PSO::measureTime(thread_argv *targs){
    if (targs->tid == threading::T_BOSS) {
        targs->timeMeasures.overallTime
                += targs->timeMeasures.lastFitnessTime
                   + targs->timeMeasures.lastNeighbourTime
                   + targs->timeMeasures.lastParticleUpdateTime;
        double averageTimeOfIteration;
        if(*(targs->curr_iter) != 0) {
            averageTimeOfIteration =
                    targs->timeMeasures.overallTime / *(targs->curr_iter);
        }
        targs->timeMeasures.ETA
                = averageTimeOfIteration *
                  (this->maxIterations - (*(targs->curr_iter)));
    }
}

void PSO::runEndingCriteria(thread_argv *targs){
    if (targs->tid == threading::T_BOSS) {
        *(targs->do_work) = !hasConvereged((targs->curr_iter));
        *(targs->curr_iter) += 1;

        if(targs->noFitnessUpdateInARowCount >=
           targs->noFitnessUpdateInARowCountBeforeReset) {
            resetCurrentGenerationParticles();
            targs->noFitnessUpdateInARowCount = 0;
        }
    }
}

bool PSO::hasConvereged(const int* iter){
    return (*iter >= this->maxIterations);
}

bool PSO::updateBestParticle(){
    bool hasChanged = false;

    if(currentGenerationBestParticle == NULL) {
        currentGenerationBestParticle = (*particles)[0];
    }

    for(unsigned int i = 0; i < swarmSize; i++){
        if((*particles)[i]->getBestFitness()
           < currentGenerationBestParticle->getBestFitness()){
            currentGenerationBestParticle = (*particles)[i];
            hasChanged = true;
        }
    }

    if(currentGenerationBestParticle->getBestFitness()
       <= globalBestParticle.getBestFitness()) {
        globalBestParticle = *currentGenerationBestParticle;
    }

    return hasChanged;
}


void PSO::resetCurrentGenerationParticles() {
    for(unsigned int i = 0; i < particles->size(); i++){
        ParticleShPtr particle = (*particles)[i];

        double posIntervalMin = particle->getPosIntervalMin();
        double posIntervalMax = particle->getPosIntervalMax();

        double velIntervalMin = particle->getVelIntervalMin();
        double velIntervalMax = particle->getVelIntervalMax();

        int particleDimension = particle->getPosition().size();

        Point<double> position(particleDimension);
        Point<double> velocity(particleDimension);

        for(unsigned int j = 0; j < particleDimension; j++){
            position[j] = rnd::generateRandomNumber(posIntervalMin,
                                                    posIntervalMax);
            velocity[j] = rnd::generateRandomNumber(velIntervalMin,
                                                    velIntervalMax);
        }

        particle->setPosition(position);
        particle->setVelocity(velocity);

        particle->setPbest(position);
        particle->setLbest(position);

        particle->setBestFitness(99999);

    }
}

//-----------------------------------------------------------//
//  FRIEND METHODS
//-----------------------------------------------------------//

void *startParallelPSO(void *argv) {
    thread_argv *targs = (thread_argv *) argv;

    targs->pso->compute(targs);
    return NULL;
}
}
