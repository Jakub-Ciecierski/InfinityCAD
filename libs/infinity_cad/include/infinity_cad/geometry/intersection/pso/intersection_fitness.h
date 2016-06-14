//
// Created by jakub on 6/13/16.
//

#ifndef IC_INTERSECTION_FITNESS_H
#define IC_INTERSECTION_FITNESS_H

#include <pso/pso_common.h>
#include <pso/fitness_updater.h>
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

class IntersectionFitness : public pso::FitnessUpdater {
private:
    Surface* surface1;
    Surface* surface2;

protected:

    virtual double fitnessValue(const pso::Particle& p) override;

public:

    IntersectionFitness(pso::ParticleShPtr_ConstVectorShPtr particles,
                        const pso::ParticleDecoder * particleDecode,
                        Surface* surface1, Surface* surface2);

    ~IntersectionFitness();

    void actOn(pso::Particle& particle) override;
};


#endif //IC_INTERSECTION_FITNESS_H
