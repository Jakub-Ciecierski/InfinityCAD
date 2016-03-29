//
// Created by jakub on 1/6/16.
//

#ifndef BACHELOR_THESIS_NEIGHBOURHOOD_UPDATER_TEST_H
#define BACHELOR_THESIS_NEIGHBOURHOOD_UPDATER_TEST_H

#include <pso/neighbourhood_updater.h>

using namespace pso;

class NeighbourhoodUpdaterTest : public NeighbourhoodUpdater{
private:


public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    NeighbourhoodUpdaterTest(ParticleShPtr_ConstVectorShPtr particles):
            NeighbourhoodUpdater(particles){

    }

    ~NeighbourhoodUpdaterTest(){}

    virtual void update() override {

    }

};


#endif //BACHELOR_THESIS_NEIGHBOURHOOD_UPDATER_TEST_H
