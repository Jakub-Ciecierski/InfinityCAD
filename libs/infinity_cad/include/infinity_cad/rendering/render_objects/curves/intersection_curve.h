//
// Created by jakub on 6/14/16.
//

#ifndef IC_INTERSECTION_CURVE_H
#define IC_INTERSECTION_CURVE_H


#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

class IntersectionCurve : public RenderObject{
private:
    std::vector<glm::vec3> vertices_vec3;

    Surface* surface1;
    Surface* surface2;
public:

    IntersectionCurve(SceneID id, std::string name,
                      std::vector<glm::vec3>& vertices,
                      Surface* surface1, Surface* surface2);

    ~IntersectionCurve();

protected:
    virtual void initVertices();

    virtual void initEdges();
};


#endif //IC_INTERSECTION_CURVE_H
