//
// Created by jakub on 3/27/16.
//

#ifndef MG1_CONE_H
#define MG1_CONE_H

#include <gm/rendering/render_body.h>

class Cone : public RenderBody{
private:
    float radius;
    float height;

    int baseVerticesCount;

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

public:

    Cone(SceneID id,
         float radius, float height, int baseVerticesCount);
    Cone(SceneID id, std::string name,
         float radius, float height, int baseVerticesCount);

    ~Cone();

    virtual float intersect(const RayCast &ray) override;

    virtual glm::vec3 getClosestPoint(const glm::vec3 point) override;
};


#endif //MG1_CONE_H
