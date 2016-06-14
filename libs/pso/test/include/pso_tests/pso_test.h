//
// Created by jakub on 1/6/16.
//

#ifndef BACHELOR_THESIS_PSO_T_TEST_H
#define BACHELOR_THESIS_PSO_T_TEST_H

#include <pso/pso.h>
#include <pso/pso_common.h>
#include <pso/particle_decoder.h>
#include <pso/particle_factory.h>
#include <pso_tests/particle_decoder_test.h>
#include <pso_tests/fitness_updater_test.h>
#include <pso_tests/neighbourhood_updater_test.h>
#include <pso_tests/particle_updater_test.h>

/*
 * Creates a specific small particle
 * Word: 011 ends in state 2.
 * Word: 0 ends in state 1.
 */
pso::Particle createParticle_s4_r2();

PSO createPSOInstance();

#endif //BACHELOR_THESIS_PSO_T_TEST_H
