//
// Created by jakub on 6/13/16.
//

#include <pso/entities/particle.h>
#include <pso/particle_decoder.h>
#include <infinity_cad/geometry/intersection/pso/intersection_fitness.h>
#include <infinity_cad/geometry/intersection/pso/param_pso_object.h>
#include <infinity_cad/math/math.h>

using namespace pso;

IntersectionFitness::IntersectionFitness(
        pso::ParticleShPtr_ConstVectorShPtr particles,
        const pso::ParticleDecoder *particleDecoder,
        Surface* surface1, Surface* surface2) :
    FitnessUpdater(particles, particleDecoder),
    surface1(surface1), surface2(surface2){

}

IntersectionFitness::~IntersectionFitness() {

}

double IntersectionFitness::fitnessValue(const pso::Particle &p) {
    ParamPSOObject* psoObject
            = (ParamPSOObject*)this->particleDecoder->decodeCurrent(p);

    glm::vec3 point1 = surface1->compute(psoObject->param1.x,
                                         psoObject->param1.y);

    glm::vec3 point2 = surface2->compute(psoObject->param2.x,
                                         psoObject->param2.y);

    double dist = ifc::euclideanDistance(point1, point2);

    delete psoObject;

    return dist;
}

void IntersectionFitness::actOn(pso::Particle &particle) {
    /*
    ParamPSOObject* psoObject
            = (ParamPSOObject*)this->particleDecoder->decodeCurrent(particle);

    glm::vec3 point1 = surface1->compute(psoObject->param1.x,
                                         psoObject->param1.y);

    glm::vec3 point2 = surface2->compute(psoObject->param2.x,
                                         psoObject->param2.y);

    glm::vec4 ndc1 = *(surface1->VPMatrix) * glm::vec4(point1.x, point1.y,
                                                       point1.z, 1.0f);
    glm::vec4 ndc2 = *(surface1->VPMatrix) * glm::vec4(point2.x, point2.y,
                                                       point2.z, 1.0f);


    ndc1.x /= ndc1.w;
    ndc1.y /= ndc1.w;

    ndc2.x /= ndc2.w;
    ndc2.y /= ndc2.w;

    glColor4f(1.0, 0.0, 0.0, 1.0f);
    glBegin(GL_POINTS);
    if(ndc1.w > 0)
        glVertex2f(ndc1.x, ndc1.y);
    if(ndc2.w > 0)
        glVertex2f(ndc2.x, ndc2.y);
    glEnd();
    */
}
