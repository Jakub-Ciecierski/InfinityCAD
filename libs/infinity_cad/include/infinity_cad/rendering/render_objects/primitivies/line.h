//
// Created by jakub on 4/2/16.
//

#ifndef MG1_LINE_H
#define MG1_LINE_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <string>

class Line : public RenderObject {
private:
    glm::vec4 v1;
    glm::vec4 v2;

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

public:
    Line(SceneID id ,
         glm::vec4 v1, glm::vec4 v2);
    Line(SceneID id , std::string name,
         glm::vec4 v1, glm::vec4 v2);

    Line(SceneID id , std::string name,
         glm::vec3 v1, glm::vec3 v2);

    ~Line();

};


#endif //MG1_LINE_H
