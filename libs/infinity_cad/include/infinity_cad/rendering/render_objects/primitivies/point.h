//
// Created by jakub on 3/23/16.
//

#ifndef MG1_POINT_H
#define MG1_POINT_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <string>

namespace ifc {

    class Point : public RenderObject {
    private:
        float radius;
        float toleratedRadius;

    protected:
        virtual void initVertices() override;
        virtual void initEdges() override;

    public:
        Point(SceneID id);
        Point(SceneID id, std::string name);
        ~Point();

        float intersect(const RayCast &ray) override;
    };
}

#endif //MG1_POINT_H
