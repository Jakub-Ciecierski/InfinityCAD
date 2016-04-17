//
// Created by jakub on 3/29/16.
//

#ifndef MG1_CLOUD_H
#define MG1_CLOUD_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <string>

class Cloud : public RenderObject {
private:
    std::vector<glm::vec4> worldVertices;

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

    virtual void drawLines(bool costumColor) override;

    virtual void transform(const glm::mat4 &VP) override;
public:

    Cloud(SceneID id,
          std::vector<glm::vec4>& points);
    Cloud(SceneID id, std::string name,
          std::vector<glm::vec4>& points);

    ~Cloud();
    const std::vector<glm::vec4>& getWorldVertices();

};


#endif //MG1_CLOUD_H
