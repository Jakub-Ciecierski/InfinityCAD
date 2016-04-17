//
// Created by jakub on 3/5/16.
//

#ifndef MG1_CAMERA_H
#define MG1_CAMERA_H

#include <infinity_cad/rigid_object.h>
#include <infinity_cad/rendering/projections/projection.h>

class Camera : public RigidObject {
protected:
    Projection* projection;
    glm::mat4 VP;

public:

    Camera(SceneID id,
           Projection* projection);
    Camera(SceneID id, std::string name,
           Projection* projection);

    virtual ~Camera();

    Projection* getProjection();
    virtual const glm::mat4& getVPMatrix() = 0;
    virtual const glm::mat4& getViewMatrix() = 0;

    virtual const glm::vec3& getDirection() = 0;
};


#endif //MG1_CAMERA_H
