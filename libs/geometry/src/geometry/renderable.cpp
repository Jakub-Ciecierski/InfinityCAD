//
// Created by jakub on 2/28/16.
//

#include <geometry/renderable.h>
#include <GL/gl.h>

using namespace glm;
using namespace std;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Renderable::Renderable() {
}

Renderable::~Renderable() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

const vector<vec4>& Renderable::getVertices() {
    return this->vertices;
}

const vector<Edge>& Renderable::getEdges() {
    return this->edges;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void Renderable::render(const mat4 &VP) {
    // TODO split transformation and drawing
    mat4 modelMatrix = getModelMatrix();
    mat4 MVP = VP * modelMatrix;
    const vector<vec4>& vertices = getVertices();

    unsigned int verticesCount = vertices.size();
    vector<vec4> transformedVertices(verticesCount);

    for(unsigned int i = 0; i < verticesCount; i++){
        transformedVertices[i] = MVP * vertices[i];

        transformedVertices[i].x /= transformedVertices[i].w;
        transformedVertices[i].y /= transformedVertices[i].w;
        transformedVertices[i].z /= transformedVertices[i].w;

    }

    const vector<Edge>& edges = getEdges();

    // Draw
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    for(unsigned int i = 0; i < edges.size(); i++){
        unsigned int index1 = edges[i].getVertex1();
        unsigned int index2 = edges[i].getVertex2();
        vec4 v1 = transformedVertices[index1];
        vec4 v2 = transformedVertices[index2];

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
