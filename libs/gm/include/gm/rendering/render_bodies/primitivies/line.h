//
// Created by jakub on 4/2/16.
//

#ifndef MG1_LINE_H
#define MG1_LINE_H

#include <gm/rendering/render_body.h>

class Line : public RenderBody{
private:
    glm::vec4 v1;
    glm::vec4 v2;

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

public:
    Line(glm::vec4 v1, glm::vec4 v2);

    ~Line();

    virtual float intersect(const RayCast &ray) override;

    virtual glm::vec3 getClosestPoint(const glm::vec3 point) override;

};


#endif //MG1_LINE_H
