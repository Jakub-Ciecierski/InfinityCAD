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

TEST(Camera, Translate) {
    glm::mat4 myMatrix = glm::mat4(1.0f);
    myMatrix[3][0] = 20.0f;
    myMatrix[3][1] = 10.0f;
    //myMatrix[0].z = 10.0f;

    glm::vec4 myVector(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 transformedVector = myMatrix * myVector;

    for(int i = 0; i < 4; i++){
        std::cout
        << myMatrix[i].x << ", "
        << myMatrix[i].y << ", "
        << myMatrix[i].z << ", "
        << myMatrix[i].w << ", "
        << std::endl;
    }


    std::cout << "Transformed Vector"
    << transformedVector.x << ", "
    << transformedVector.y << ", "
    << transformedVector.z << ", "
    << transformedVector.w << ", "
    << std::endl;
}