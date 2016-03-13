//
// Created by jakub on 3/5/16.
//

#ifndef MG1_CAMERA_FPS_H
#define MG1_CAMERA_FPS_H

#include <gm/object.h>
#include <gm/projections/projection.h>
#include <gm/cameras/camera.h>

class CameraFPS : public Camera{
private:
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

    glm::mat4 viewMatrix;

public:
    float horizontalAngleDegree;
    float verticalAngleDegree;

    float buttonsSpeed;
    float mouseSpeed;

    CameraFPS();

    CameraFPS(Projection* projection);

    ~CameraFPS();

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void moveForward(float speedBoost);
    void moveBackward(float speedBoost);
    void moveLeft(float speedBoost);
    void moveRight(float speedBoost);
    void moveUp(float speedBoost);
    void moveDown(float speedBoost );

    const glm::mat4& getVPMatrix();
    virtual const glm::mat4 &getViewMatrix();
    virtual void update() override;
};


#endif //MG1_CAMERA_FPS_H
