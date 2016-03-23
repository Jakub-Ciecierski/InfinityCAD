//
// Created by jakub on 3/23/16.
//

#ifndef MG1_POINT_H
#define MG1_POINT_H


#include <gm/rendering/render_body.h>

class Point : public RenderBody {
private:
    float radius;
    float toleratedRadius;

protected:
    virtual void drawLines(const std::vector<glm::vec4> &vertices,
                           bool costumColor) override;

    virtual void initVertices() override;
    virtual void initEdges() override;

public:

    Point();
    ~Point();

    float intersect(const RayCast& ray) override;
};


#endif //MG1_POINT_H
