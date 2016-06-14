//
// Created by kuba on 1/8/16.
//

#include <pso/particle_updaters/particle_updater_spherical.h>
#include <math/hyper_sphere.h>
#include <math/geometry.h>

using namespace pso;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

ParticleUpdaterSpherical::ParticleUpdaterSpherical(
                        ParticleShPtr_ConstVectorShPtr particles,
                        double learningFactor, double velocityWeight,
                        int timesToAttemptPointGenerationWithinHyperSphere) :
                ParticleUpdater(particles){
    this->learningFactor = learningFactor;
    this->velocityWeight = velocityWeight;

    this->timesToAttemptPointGenerationWithinHyperSphere
            = timesToAttemptPointGenerationWithinHyperSphere;
}

ParticleUpdaterSpherical::~ParticleUpdaterSpherical(){

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void ParticleUpdaterSpherical::update(int startIndex, int finishIndex){

    for(int i = startIndex; i <= finishIndex; i++){
        ParticleShPtr particle = (*particles)[i];

        this->updateParticle(*particle);

    }
}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

void ParticleUpdaterSpherical::updateParticle(Particle& particle){
    const Point<double>& currentPosition = particle.getPosition();
    const Point<double>& pbest = particle.getPbest();
    const Point<double>& lbest = particle.getLbest();
    const Point<double>& currentVelocity = particle.getVelocity();

    HyperSphere* hyperSphere = generateHyperSphere(currentPosition,
                                                   pbest,lbest);
    Point<double>* randomPointWithin
            = generateRandomPointWithinHyperSphere(*hyperSphere,
                         timesToAttemptPointGenerationWithinHyperSphere);

    Point<double> weightedVelocity = (currentVelocity*velocityWeight);
    Point<double> newVelocity = weightedVelocity +
                    ((*randomPointWithin) - currentPosition);
    Point<double> newPosition = weightedVelocity + (*randomPointWithin);

    newPosition = math::boundPoint(currentPosition,
                                   newPosition,
                                   particle.getMaxVelocity());

    particle.setVelocity(newVelocity);
    particle.setPosition(newPosition);

    this->boundParticleWithinSolutionSpace(particle);

    delete hyperSphere;
    delete randomPointWithin;
}

HyperSphere*ParticleUpdaterSpherical::generateHyperSphere(
                                    const Point<double>& currentPosition,
                                    const Point<double>& pbest,
                                    const Point<double>& lbest){
    int pointCount = 3;
    int positionMultiplier = 2;

    Point<double>* centerOfGravity;

    // if pbest == lbest, skip lbest in computations
    if(pbest == lbest){
        pointCount = 2;
        positionMultiplier = 1;

        Point<double> emptyPoint(currentPosition.size());

        centerOfGravity =
                this->centerOfGravity(currentPosition,
                                      pbest, emptyPoint,
                                      learningFactor,
                                      pointCount, positionMultiplier);
    }else{
        centerOfGravity =
                this->centerOfGravity(currentPosition,
                                      pbest, lbest,
                                      learningFactor,
                                      pointCount, positionMultiplier);
    }

    double radius = math::euclideanDistance(*centerOfGravity,
                                            currentPosition);

    HyperSphere* hyperSphere = new HyperSphere(*centerOfGravity, radius);

    delete centerOfGravity;

    return hyperSphere;
}

Point<double>*ParticleUpdaterSpherical::centerOfGravity(
                                const Point<double>& currentPosition,
                                const Point<double>& pbest,
                                const Point<double>& lbest,
                                double learningFactor,
                                int pointCount,
                                int positionMultiplier){
    Point<double>* centerOfGravity = new Point<double>(
            (currentPosition +
             (( pbest  + lbest  -
                (currentPosition*positionMultiplier ) ) / pointCount)
             * learningFactor)
    );

    return centerOfGravity;
}

Point<double>*ParticleUpdaterSpherical::generateRandomPointWithinHyperSphere(
        const HyperSphere& hyperSphere, int numberOfTries){
    Point<double>* randomPointWithin;

    do {
        randomPointWithin
                = hyperSphere.randomPointWithinDescribedHyperCube();
    }while(numberOfTries-- > 0
           && !(hyperSphere.isInSphere(*randomPointWithin)));

    return randomPointWithin;
}