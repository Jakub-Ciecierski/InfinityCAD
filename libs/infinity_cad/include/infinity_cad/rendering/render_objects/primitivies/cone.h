//
// Created by jakub on 3/27/16.
//

#ifndef MG1_CONE_H
#define MG1_CONE_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <string>

class Cone : public RenderObject {
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
};


#endif //MG1_CONE_H
