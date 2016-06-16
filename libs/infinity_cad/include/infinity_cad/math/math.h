//
// Created by jakub on 4/16/16.
//

#ifndef IC_MATH_H
#define IC_MATH_H

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <functional>

namespace ifc {
    const float PI_OVER_180 = M_PI/180;

    float dot(const glm::vec3 &v1, const glm::vec3 &v2);

    float euclideanDistance(const glm::vec4& v1, const glm::vec4& v2);
    float euclideanDistance(const glm::vec3& v1, const glm::vec3& v2);
    float euclideanDistance(const glm::vec2& v1, const glm::vec2& v2);

    float angleToRadians(float angle);

    float radiansToDegree(float rad);

    bool equal(const glm::vec3& v1, const glm::vec3& v2, float error);

    void solveTridiagonalSystem(std::vector<float>&belowDiagonal, std::vector<float>&mainDiagonal,
                                std::vector<float>&aboveDiagonal, std::vector<float>& d);

    float xPixelToGLCoord(int p, int width);
    float yPixelToGLCoord(int p, int height);
    int xGLToPixelCoord(float x, int width);
    int yGLToPixelCoord(float y, int height);

    float getMultplicationValue(const glm::vec4& v1,
                                const glm::mat4& m,
                                const glm::vec4& v2);

    void printVec2(const glm::vec2& vec);
    void printVec3(const glm::vec3& vec);
    void printVec4(const glm::vec4& vec);
    bool isNan(const glm::vec4& v);

    enum DerivativeTypes {
        DY, DX, DZ, DW
    };
    float derivative(std::function<float(float, float, float, float)> f,
                     float x, float y, float z, float w,
                     DerivativeTypes derivativeType);
}

#endif //IC_MATH_H
