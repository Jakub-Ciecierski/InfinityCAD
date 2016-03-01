//
// Created by jakub on 2/28/16.
//

#ifndef MG1_OBJECT_H
#define MG1_OBJECT_H

#include <glm/gtc/matrix_transform.hpp>

/*
 * Object can be moved, rotated and scaled.
 * GLM matrices and vertices are used for computating.
 * It is important to note the structure of these objects:
 * A matrix T = |1 0 0 X|
 *              |0 1 0 Y|
 *              |0 0 0 Z|
 *              |0 0 0 1|
 * Will be accessed as follows:
 * T[0] = |1|
 *        |0|
 *        |0|
 *        |0|
 * T[3] = |X|
 *        |Y|
 *        |Z|
 *        |1|
 * In other words, the matrices store columns linearly.
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
