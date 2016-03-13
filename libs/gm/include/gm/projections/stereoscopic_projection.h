//
// Created by jakub on 3/11/16.
//

#ifndef MG1_STEREOSCOPIC_PROJECTION_H
#define MG1_STEREOSCOPIC_PROJECTION_H


#include <gm/color/color.h>
#include "gm/projections/perspective_projection.h"

class StereoscopicProjection : public PerspectiveProjection{
public:

    StereoscopicProjection(float projectionDistance, float distance3D);
    StereoscopicProjection(float projectionDistance, float distance3D,
                           Color leftColor, Color rightColor);

    ~StereoscopicProjection();

    void setDistance3D(float distance3D);

    void setLeftColor(Color c);
    void setRightColor(Color c);

    const glm::mat4 &getLeftProjectionMatrix() const;
    const glm::mat4 &getRightProjectionMatrix() const;

    const Color& getLeftColor() const;
    const Color& getRightColor() const;

    virtual void update() override;
private:

    float distance3D;

    glm::mat4 leftProjectionMatrix;
    glm::mat4 rightProjectionMatrix;

    Color leftColor;
    Color rightColor;
};


#endif //MG1_STEREOSCOPIC_PROJECTION_H
