//
// Created by jakub on 3/27/16.
//

#ifndef MG1_AXIS_NET_H
#define MG1_AXIS_NET_H

#include <gm/rendering/render_body.h>
#include <gm/rendering/render_bodies/primitivies/line.h>

class AxisNet : public RenderBody{
private:
    std::vector<Line> lines;

    float middleLineWidth;
    int netSize;
    Color middleLineColor;

    void initLine(glm::vec4 v1, glm::vec4 v2,
                  int& currentVertex,
                  const Color* color, float width = 1.0f);

    void initLineSegment(glm::vec4 v1, glm::vec4 v2,
                         int& currentVertex,
                         const Color* color, float width = 1.0f);

    void initXLines(float start, float finish,
                    float deltaDist, int& currentVertex);
    void initZLines(float start, float finish,
                    float deltaDist, int& currentVertex);

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;


public:

    AxisNet(int netSize);
    ~AxisNet();

    virtual float intersect(const RayCast &ray) override;

    virtual glm::vec3 getClosestPoint(const glm::vec3 point) override;

    virtual void render(const glm::mat4 &VP) override;

    virtual void render(const glm::mat4 &VP, const Color &color) override;

    virtual void update() override;
};


#endif //MG1_AXIS_NET_H
