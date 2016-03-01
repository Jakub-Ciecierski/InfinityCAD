//
// Created by jakub on 2/28/16.
//

#ifndef MG1_OBJECT_H
#define MG1_OBJECT_H

#include <glm/gtc/matrix_transform.hpp>

/*
 * Object can be moved, rotated and scaled
 */
class Object {
private:
    glm::mat4 modelMatrix;

    glm::mat4 getTranslationMatrix();

    /*
     * RotateMatrix = XRotate * YRotate * ZRotate
     */
    glm::mat4 getRotationMatrix();
    glm::mat4 getXRotationMatrix();
    glm::mat4 getYRotationMatrix();
    glm::mat4 getZRotationMatrix();

    glm::mat4 getScaleMatrix();

protected:
    glm::vec3 position;
    glm::vec3 rotationAngles;
    float scaleFactor;

    /*
     * ModelMatrix = Translate * Rotate * Scale
     */
    const glm::mat4& getModelMatrix();

    /*
     * Clamp angles within a fixed interval [0, 360]
     */
    void clampAngles();

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Object();

    virtual ~Object();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void move(float dx, float dy, float dz);

    void moveTo(float x, float y, float z);

    void rotate(float dxAngle, float dyAngle, float dzAngle);

    void scale(float scale);

    void scaleDt(float scale);
};

#endif //MG1_OBJECT_H
