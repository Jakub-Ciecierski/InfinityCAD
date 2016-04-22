//
// Created by jakub on 2/28/16.
//

#ifndef MG1_RIGID_BODY_H
#define MG1_RIGID_BODY_H

#include <glm/gtc/matrix_transform.hpp>
#include "infinity_cad/object.h"


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
class RigidObject : public ifc::Object{
private:
    glm::mat4 modelMatrix;

    glm::mat4 constructTranslationMatrix();

    /*
     * RotateMatrix = XRotate * YRotate * ZRotate
     */
    glm::mat4 constructRotationMatrix();
    glm::mat4 constructXRotationMatrix();
    glm::mat4 constructYRotationMatrix();
    glm::mat4 constructZRotationMatrix();

    glm::mat4 constructScaleMatrix();

protected:
    glm::vec3 position;
    glm::vec3 rotationAngles;
    float scaleFactor;

    /*
     * Clamp angles within a fixed interval [0, 360]
     */
    void clampAngles();

    virtual void translate(float x, float y, float z);

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    RigidObject(SceneID id);
    RigidObject(SceneID id, std::string name);

    virtual ~RigidObject();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void move(float dx, float dy, float dz);
    void moveTo(float x, float y, float z);
    void moveTo(const glm::vec3& pos);
    void moveTo(const RigidObject * toBody);

    void rotate(float dxAngle, float dyAngle, float dzAngle);
    void rotateTo(float xAngle, float yAngle, float zAngle);


    void scale(float scale);
    void scaleDt(float scale);

    const glm::vec3& getRotations() const;
    const glm::vec3& getPosition() const;

    virtual glm::vec3 getClosestPoint(const glm::vec3& point);

    /*
     * ModelMatrix = Translate * Rotate * Scale
     */
    const glm::mat4& getModelMatrix();

    virtual void update();
};

#endif //MG1_RIGID_BODY_H
