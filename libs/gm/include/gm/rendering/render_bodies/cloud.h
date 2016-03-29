//
// Created by jakub on 3/29/16.
//

#ifndef MG1_CLOUD_H
#define MG1_CLOUD_H

#include <gm/rendering/render_body.h>


class Cloud : public RenderBody{
private:
    std::vector<glm::vec4> worldVertices;

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

    virtual void drawLines(const std::vector<glm::vec4> &vertices,
                           bool costumColor) override;

    virtual void transform(const glm::mat4 &VP) override;
public:

    Cloud(std::vector<glm::vec4>& points);
    ~Cloud();
    const std::vector<glm::vec4>& getWorldVertices();

    virtual float intersect(const RayCast &ray) override;

    virtual glm::vec3 getClosestPoint(const glm::vec3 point) override;

};


#endif //MG1_CLOUD_H
