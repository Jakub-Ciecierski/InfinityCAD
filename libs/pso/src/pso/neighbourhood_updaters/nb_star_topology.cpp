//
// Created by jakub on 1/10/16.
//

#include <pso/neighbourhood_updaters/nb_star_topology.h>
#include <pso/entities/particle.h>
#include <math/random.h>

using namespace pso;
using namespace std;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

NBStarTopology::NBStarTopology(ParticleShPtr_ConstVectorShPtr particles,
                               unsigned int numberOfParticlesToInform) :
                NeighbourhoodUpdater(particles){
    this->numberOfParticlesToInform = numberOfParticlesToInform;

    minParticleIndex = 0;
    maxParticleIndex = particles->size() - 1;
}

NBStarTopology::~NBStarTopology() {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void NBStarTopology::update() {
    unsigned int size = particles->size();

    for(unsigned int particleIndex = 0; particleIndex < size; particleIndex++){
        vector<int>* neighbours = this->selectNeighbourhood(particleIndex);

        int bestIndex = selectBestNeighbour(neighbours);


        ParticleShPtr bestNeighbour = ((*particles)[bestIndex]);
        ParticleShPtr currentParticle = ((*particles)[particleIndex]);

        currentParticle->setLbest(bestNeighbour->getPbest());

        delete neighbours;
    }


}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

vector<int>* NBStarTopology::selectNeighbourhood(unsigned int particleIndex){
    vector<int>* neighbours = new vector<int>();

    neighbours->push_back(particleIndex);

    for(unsigned int t = 0; t < numberOfParticlesToInform; t++){
        int neighbourIndex
                = rnd::generateRandomNumber(minParticleIndex,
                                              maxParticleIndex+1);
        neighbours->push_back(neighbourIndex);
    }

    return neighbours;
}

int NBStarTopology::selectBestNeighbour(vector<int>* neighbours){
    unsigned int neighboursCount = neighbours->size();
    int bestIndex = (*neighbours)[0];
    double neighbourhoodBestFitness
            = ((*particles)[bestIndex])->getBestFitness();

    for(unsigned int i = 0; i < neighboursCount; i++){
        int neighbourIndex = (*neighbours)[i];

        double particleBestFitness
                = ((*particles)[neighbourIndex])->getBestFitness();

        if(particleBestFitness > neighbourhoodBestFitness){
            bestIndex = neighbourIndex;
            neighbourhoodBestFitness = particleBestFitness;
        }
    }

    return bestIndex;
}