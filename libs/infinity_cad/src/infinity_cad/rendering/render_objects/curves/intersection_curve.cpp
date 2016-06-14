//
// Created by jakub on 6/14/16.
//

#include <infinity_cad/rendering/render_objects/curves/intersection_curve.h>
#include <infinity_cad/rendering/color/color_settings.h>

using namespace glm;

IntersectionCurve::IntersectionCurve(SceneID id, std::string name,
                                     std::vector<glm::vec3>& vertices,
                                     Surface *surface1, Surface *surface2) :
        RenderObject(id, name), vertices_vec3(vertices),
        surface1(surface1), surface2(surface2){
    initVertices();
    initEdges();

    setColor(COLOR_OBJECT_DEFAULT);
    lineWidth = 1.5f;
}

IntersectionCurve::~IntersectionCurve() {

}

void IntersectionCurve::initVertices() {
    for(unsigned int i = 0; i < vertices_vec3.size(); i++){
        vec3 vec = vertices_vec3[i];
        vertices.push_back(vec4(vec.x, vec.y, vec.z, 1.0f));
    }
}

void IntersectionCurve::initEdges() {
    for(unsigned int i = 1; i < vertices_vec3.size(); i++){
        edges.push_back(Edge(i-1, i));
    }
}
