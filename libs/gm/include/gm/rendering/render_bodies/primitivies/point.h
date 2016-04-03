//
// Created by jakub on 3/23/16.
//

#ifndef MG1_POINT_H
#define MG1_POINT_H


#include <gm/rendering/render_body.h>

namespace ic {

    class Point : public RenderBody {
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

        virtual glm::vec3 getClosestPoint(const glm::vec3 point) override;
    };
}

#endif //MG1_POINT_H
