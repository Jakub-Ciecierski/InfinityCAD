//
// Created by jakub on 3/23/16.
//

#ifndef MG1_RAY_CAST_H
#define MG1_RAY_CAST_H

#include <gm/cameras/camera.h>

class Camera;

const float RAYCAST_NO_SOLUTION = 99999.9f;

class RayCast {
private:
    Camera* camera;

    glm::vec3 origin;
    glm::vec3 direction;
public:
    float currentX;
    float currentY;

    RayCast(Camera* camera);
    ~RayCast();

    const glm::vec3& getOrigin() const;
    const glm::vec3& getDirection() const;

    void update(float x, float y);

    void render(const glm::mat4& MVP) const;
};


#endif //MG1_RAY_CAST_H
