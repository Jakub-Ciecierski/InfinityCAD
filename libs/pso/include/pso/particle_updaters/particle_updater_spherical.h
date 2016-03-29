//
// Created by kuba on 1/8/16.
//

#ifndef BACHELOR_THESIS_PARTICLE_UPDATER_NAIVE_H
#define BACHELOR_THESIS_PARTICLE_UPDATER_NAIVE_H

#include <pso/entities/particle.h>
#include <pso/particle_updater.h>
#include <pso/pso_common.h>
#include <math/hyper_sphere.h>

namespace pso {
/*
 * Particle Updater as described in PSO11:
 * https://hal.archives-ouvertes.fr/hal-00764996/document
 *
 * Each Particle is updated by creating a random point within a hypersphere.
 * Hypersphere:
 *  - center is center of gravity G between three points:
 *      - current position
 *      - difference of pbest and current point, times learning constant
 *      - difference of lbest and current point, times learning constant
 *  - radius:
 *      - Distance between G and current position.
 *
 */
class ParticleUpdaterSpherical : public ParticleUpdater {
private:

    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    double learningFactor;

    double velocityWeight;

    // Generate point within hyper sphere by generating it first
    // within hyper cube, and check if it lies in the hyper sphere.
    // This variable bounds the amount
    // of times we try to generate such point
    int timesToAttemptPointGenerationWithinHyperSphere;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    /*
     * Updates a single particle.
     */
    void updateParticle(Particle& particle);

    /*
     * Generates Hyper Sphere.
     * Allocates memory.
     */
    HyperSphere* generateHyperSphere(const Point<double>& currentPosition,
                                     const Point<double>& pbest,
                                     const Point<double>& lbest);

    /*
     * Allocates memory for the point, must be deleted manually.
     */
    Point<double>* centerOfGravity(
            const Point<double>& currentPosition,
            const Point<double>& pbest,
            const Point<double>& lbest,
            double learningFactor,
            int pointCount,
            int positionMultiplier);

    /*
     * Tries to generate random point within hyper sphere by
     * generating point within described hypercube and checking
     * if this point is within hyper sphere, maximum numberOfTries times.
     *
     * Uses: hyperSphere.randomPointWithinDescribedHyperCube()
     * which allocated memory.
     */
    Point<double>* generateRandomPointWithinHyperSphere(
            const HyperSphere& hyperSphere, int numberOfTries);

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    ParticleUpdaterSpherical(ParticleShPtr_ConstVectorShPtr particles,
                            double learningFactor, double velocityWeight,
                            int timesToAttemptPointGenerationWithinHyperSphere);

    virtual ~ParticleUpdaterSpherical();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual void update(int startIndex, int finishIndex) override;
};

}

#endif //BACHELOR_THESIS_PARTICLE_UPDATER_NAIVE_H
