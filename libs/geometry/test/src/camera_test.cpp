//
// Created by jakub on 2/27/16.
//

#include <gtest/gtest.h>
#include <geometry/cameras/camera.h>

TEST(Camera, UpdateMVP) {
    Camera camera;
    const glm::mat4& viewMatrix = camera.getViewMatrix();

    for(int i = 0; i < 4; i++){
        std::cout
        << viewMatrix[i].x << ", "
        << viewMatrix[i].y << ", "
        << viewMatrix[i].z << ", "
        << viewMatrix[i].w << ", "
        << std::endl;
    }

}
