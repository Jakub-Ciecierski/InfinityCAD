//
// Created by jakub on 3/23/16.
//

#ifndef MG1_POINT_H
#define MG1_POINT_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <string>

namespace ifc {

enum PointSurfaceType{
    NONE, U0V0, U0V1, U1V0, U1V1
};

    class Point : public RenderObject {
    private:
        float radius;
        float toleratedRadius;

        PointSurfaceType point_surface_type_;

    protected:
        virtual void initVertices() override;
        virtual void initEdges() override;

    public:
        Point(SceneID id);
        Point(SceneID id, std::string name);
        ~Point();

        PointSurfaceType point_surface_type() {return point_surface_type_;}
        PointSurfaceType point_surface_type(
                PointSurfaceType point_surface_type){
            point_surface_type_ = point_surface_type;
        }

        float intersect(const RayCast &ray) override;
    };
}

#endif //MG1_POINT_H
