//
// Created by jakub on 3/27/16.
//

#ifndef MG1_AXIS_NET_H
#define MG1_AXIS_NET_H

#include <gm/rendering/render_body.h>

class AxisNet : public RenderBody{
private:
    int netSize;

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

    virtual void drawLines(const std::vector<glm::vec4>& vertices,
                           bool costumColor) override;

public:

    AxisNet(int netSize);

    ~AxisNet();

    virtual float intersect(const RayCast &ray) override;
};


#endif //MG1_AXIS_NET_H
