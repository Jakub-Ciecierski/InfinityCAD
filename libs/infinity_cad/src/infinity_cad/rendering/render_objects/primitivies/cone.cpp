//
// Created by jakub on 3/27/16.
//

#include <GL/gl.h>
#include <infinity_cad/rendering/render_objects/primitivies/cone.h>
#include <infinity_cad/rendering/visibility/ray_cast.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Cone::Cone(SceneID id,
           float radius, float height, int baseVerticesCount) :
        RenderObject(id),
        radius(radius),
        height(height),
        baseVerticesCount(baseVerticesCount) {
    initVertices();
    initEdges();

    drawingMode = GL_TRIANGLE_FAN;
}
Cone::Cone(SceneID id, std::string name,
           float radius, float height, int baseVerticesCount) :
        RenderObject(id, name),
        radius(radius),
        height(height),
        baseVerticesCount(baseVerticesCount) {
    initVertices();
    initEdges();

    drawingMode = GL_TRIANGLE_FAN;
}

Cone::~Cone() {

}

//-----------------------//
//  PROTECTED
//-----------------------//

void Cone::initVertices() {
    for(int i = 0; i < baseVerticesCount; i++){
        float angle = (float)i / (float)baseVerticesCount * M_PI * 2.0F;

        float x = radius * cos(angle);
        float y = 0;
        float z = radius * sin(angle);

        vertices.push_back(vec4(x, y, z, 1));
    }

    vertices.push_back(vec4(0, height, 0, 1));
}

void Cone::initEdges() {
    unsigned int i;
    for(i = 0; i < vertices.size() - 2; i++){
        edges.push_back(Edge(i, i + 1));
        edges.push_back(Edge(i, vertices.size() - 1));
    }

    edges.push_back(Edge(i, 0));
    edges.push_back(Edge(i, vertices.size() - 1));

}