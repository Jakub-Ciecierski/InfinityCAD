//
// Created by jakub on 12/29/15.
//

#ifndef BACHELOR_THESIS_PARTICLE_FACTORY_T_H
#define BACHELOR_THESIS_PARTICLE_FACTORY_T_H

#include <pso/pso_common.h>

namespace pso {

class ParticleFactory {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    int swarmSize;

    int particleDimension;

    double posIntervalMin;
    double posIntervalMax;

    double velIntervalMin;
    double velIntervalMax;

    double maxVelocity;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:

    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    ParticleFactory(int swarmSize, int particleDimension,
                      double posIntervalMin, double posIntervalMax,
                      double velIntervalMin, double velIntervalMax,
                      double maxVelocity);

    ~ParticleFactory();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    ParticleShPtr_ConstVectorShPtr createUnifromParticles();
    ParticleShPtr_ConstVectorShPtr createUnifromParticles(Point<double> position);
};
}

#endif //BACHELOR_THESIS_PARTICLE_FACTORY_T_H
