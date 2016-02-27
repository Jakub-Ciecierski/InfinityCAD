//
// Created by jakub on 2/27/16.
//

#include <gtest/gtest.h>
#include <geometry/camera.h>

TEST(Camera, UpdateMVP) {
    Camera camera(0,0,0);

    const glm::mat4& MVP = camera.getMVP();

    for(int i = 0; i < 4; i++){
        std::cout
        << MVP[i].x << ", "
        << MVP[i].y << ", "
        << MVP[i].z << ", "
        << MVP[i].w << ", "
        << std::endl;
    }

}
