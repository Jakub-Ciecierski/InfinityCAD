//
// Created by jakub on 4/19/16.
//

#include <glm/gtc/matrix_transform.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <infinity_cad/math/math.h>

using namespace std;
using namespace glm;

TEST(solveTridiagonalSystem, solvesCorrectly) {
    vector<float> a = {0, -1, -1, -1};
    vector<float> b = { 4,  4,  4,  4 };
    vector<float> c = {-1, -1, -1,  0 };
    vector<float> d = { 5,  5, 10, 23 };

    ifc::solveTridiagonalSystem(a,b,c,d);
    vector<float> exptectedValue = {2, 3 ,5 ,7};
    // results    { 2,  3,  5, 7  }

    for(unsigned int i = 0; i < d.size(); i++){
        EXPECT_EQ(exptectedValue[i], d[i]);
    }
}

TEST(GLM, mat_vec_multiplication) {
    mat4 m(1.0f);
    vec4 v(2.0f, 3.0f, 4.0f, 5.0f);
    vec4 u(1.0f, 2.0f, 3.0f, 4.0f);
}