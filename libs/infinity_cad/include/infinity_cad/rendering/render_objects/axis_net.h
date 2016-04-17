//
// Created by jakub on 3/27/16.
//

#ifndef MG1_AXIS_NET_H
#define MG1_AXIS_NET_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>
#include <infinity_cad/rendering/render_objects/primitivies/line.h>
#include <infinity_cad/rendering/color/color.h>

#include <string>


class AxisNet : public RenderObject {
private:
    std::vector<Line*> lines;

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

    AxisNet(SceneID id,
            int netSize);
    AxisNet(SceneID id, std::string name,
            int netSize);

    ~AxisNet();

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;

    virtual void update() override;
};


#endif //MG1_AXIS_NET_H
