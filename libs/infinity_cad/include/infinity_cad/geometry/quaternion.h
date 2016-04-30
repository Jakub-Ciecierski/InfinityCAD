//
// Created by jakub on 4/30/16.
//

#ifndef IC_QUATERNION_H
#define IC_QUATERNION_H

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class Quaternion {
private:
    std::vector<float> entries;

    float norm();

public:

    Quaternion(float q0, float q1, float q2, float q3);
    Quaternion(const glm::vec3& vec, float degree);
    Quaternion(const glm::vec3& vec);

    ~Quaternion();

    Quaternion conjugate();

    void normalize();

    static glm::vec3 rotate(const glm::vec3& vec, const glm::vec3& axis,
                            float degree);

    Quaternion operator* (const Quaternion&Q2) const;

    float& operator[](int i);
    const float& operator[](int i) const;
};


#endif //IC_QUATERNION_H
