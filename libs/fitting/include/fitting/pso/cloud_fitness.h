//
// Created by jakub on 3/29/16.
//

#ifndef MG1_CLOUD_FITNESS_H
#define MG1_CLOUD_FITNESS_H

#include <pso/pso_common.h>
#include <pso/fitness_updater.h>
#include <gm/rigid_body.h>
#include <gm/rendering/render_bodies/cloud.h>

class CloudFitness : public pso::FitnessUpdater{
private:
    float distancesWeight;
    float normalAnglesWeight;

    Cloud* cloud;
    RigidBody* rigidBody;

    double computeGoalFunction(Cloud* cloud, RigidBody* rigidBody);

protected:

    virtual double fitnessValue(const pso::Particle& p) override;


public:

    CloudFitness(pso::ParticleShPtr_ConstVectorShPtr particles,
                 const pso::ParticleDecoder * particleDecoder,
                 Cloud* cloud, RigidBody* rigidBody);

    virtual ~CloudFitness();

};


#endif //MG1_CLOUD_FITNESS_H
