//
// Created by jakub on 3/29/16.
//

#ifndef MG1_SPHERE_H
#define MG1_SPHERE_H

#include <gm/rendering/render_body.h>
#include <gm/rendering/render_bodies/cloud.h>

class Sphere : public RenderBody{
private:
    float radius;
    int ringsCount;
    int sectionCount;

    // s = [0, 2PI]
    // r = [0, PI]
    float getX(float s, float r);
    float getY(float s, float r);
    float getZ(float s, float r);

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

public:
    Sphere(float radius,
           int ringsCount, int sectionCount);
    ~Sphere();

    Cloud* extractRandomCloud(int verticesCount,
                              float sDivider, float rDivider);

    virtual float intersect(const RayCast &ray) override;

    virtual glm::vec3 getClosestPoint(const glm::vec3 point) override;
};


#endif //MG1_SPHERE_H