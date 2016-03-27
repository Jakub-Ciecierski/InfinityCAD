//
// Created by jakub on 3/27/16.
//

#include <gm/rendering/render_bodies/primitivies/cone.h>
#include <GL/gl.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Cone::Cone(float radius,
           float height,
           int baseVerticesCount) :
        radius(radius),
        height(height),
        baseVerticesCount(baseVerticesCount) {

    initVertices();
    initEdges();
    transformedVertices.resize(vertices.size());

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
    int i;
    for(i = 0; i < vertices.size() - 2; i++){
        edges.push_back(Edge(i, i + 1));
        edges.push_back(Edge(i, vertices.size() - 1));
    }

    edges.push_back(Edge(i, 0));
    edges.push_back(Edge(i, vertices.size() - 1));

}

void Cone::drawLines(const std::vector<glm::vec4>& vertices,
                     bool costumColor){
    const vector<Edge>& edges = getEdges();

    glBegin(GL_TRIANGLE_FAN);
    for(unsigned int i = 0; i < edges.size(); i++){
        if(costumColor && edges[i].color != NULL)
            setSurfaceColor(*(edges[i].color));

        unsigned int index1 = edges[i].getVertex1();
        unsigned int index2 = edges[i].getVertex2();
        vec4 v1 = vertices[index1];
        vec4 v2 = vertices[index2];

        // CLIPPING
        // Don't draw if either is not visiable
        if(v1.w < 0 || v2.w < 0) continue;

        // v2 is behind
        if(v2.w < 0){
            float n = v1.w / (v1.w - v2.w);
            float xc = (n * v1.x) + ((1-n) * v2.x);
            float yc = (n * v1.y) + ((1-n) * v2.y);

            v2 = vec4(xc,yc,0,0);
        }
        // v1 is behind
        if(v1.w < 0){
            float n = v2.w / (v2.w - v1.w);
            float xc = (n * v2.x) + ((1-n) * v1.x);
            float yc = (n * v2.y) + ((1-n) * v1.y);

            v1 = vec4(xc,yc,0,0);
        }
        glVertex2f(v1.x,
                   v1.y);
        glVertex2f(v2.x,
                   v2.y);
    }
    glEnd();
}

//-----------------------//
//  PUBLIC 
//-----------------------//


float Cone::intersect(const RayCast & ray){
    return RAYCAST_NO_SOLUTION;
}
