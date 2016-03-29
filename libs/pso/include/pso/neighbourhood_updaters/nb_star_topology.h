//
// Created by jakub on 1/10/16.
//

#ifndef BACHELOR_THESIS_NB_STAR_TOPOLOGY_H
#define BACHELOR_THESIS_NB_STAR_TOPOLOGY_H

#include <pso/neighbourhood_updater.h>
#include <vector>

using namespace std;

namespace pso {

/*
 * NBStarTopology is a simply method of neighbourhood update.
 *
 * For each Particle neighbourhood is created containing itself
 * and numberOfParticlesToInform other random Particles.
 * Among that neighbourhood the best Particle is found in terms of its
 * PBest. Such Pbest of best neighbour is then set as LBest of the Particle
 * in interest.
 */
class NBStarTopology : public NeighbourhoodUpdater {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    unsigned int numberOfParticlesToInform;

    int minParticleIndex;
    int maxParticleIndex;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    /*
     * Select neighbourhood of particle.
     *
     * Allocates memory
     */
    vector<int>* selectNeighbourhood(unsigned int particleIndex);

    /*
     * Returns index of best neighbours.
     * Index
     */
    int selectBestNeighbour(vector<int>* neighbours);

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    NBStarTopology(ParticleShPtr_ConstVectorShPtr particles,
                   unsigned int numberOfParticlesToInform);

    virtual ~NBStarTopology();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual void update() override;
};
}

#endif //BACHELOR_THESIS_NB_STAR_TOPOLOGY_H
