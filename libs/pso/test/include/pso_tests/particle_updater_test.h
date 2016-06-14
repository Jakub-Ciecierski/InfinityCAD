//
// Created by jakub on 1/6/16.
//

#ifndef BACHELOR_THESIS_PARTICLE_UPDATER_TEST_H
#define BACHELOR_THESIS_PARTICLE_UPDATER_TEST_H

#include <pso/particle_updater.h>

using namespace pso;

class ParticleUpdaterTest : public ParticleUpdater {
private:

public:

    ParticleUpdaterTest(ParticleShPtr_ConstVectorShPtr particles) :
            ParticleUpdater(particles){

    }

    ~ParticleUpdaterTest(){}

    virtual void update(int startIndex, int finishIndex) override {

    }

};


#endif //BACHELOR_THESIS_PARTICLE_UPDATER_TEST_H
