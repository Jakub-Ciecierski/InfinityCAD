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

    virtual void drawLines(const std::vector<glm::vec4>& vertices,
                           bool costumColor) override;

public:

    Cone(float radius, float height, int baseVerticesCount);
    ~Cone();

    virtual float intersect(const RayCast &ray) override;

};


#endif //MG1_CONE_H
