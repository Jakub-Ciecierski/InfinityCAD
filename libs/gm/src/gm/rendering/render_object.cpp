//
// Created by jakub on 2/28/16.
//


#include <GL/gl.h>
#include <gm/rendering/render_object.h>
#include <gm/color/color_settings.h>

using namespace glm;
using namespace std;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

RenderObject::RenderObject() : color(COLOR_OBJECT_DEFAULT){
}

RenderObject::~RenderObject() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

void RenderObject::setSurfaceColor(const Color &color) {
    glColor3f(color.R, color.G, color.B);
}

const vector<vec4>& RenderObject::getVertices() {
    return this->vertices;
}

const vector<Edge>& RenderObject::getEdges() {
    return this->edges;
}

void RenderObject::drawLines(const std::vector<glm::vec4>& vertices){
    const vector<Edge>& edges = getEdges();

    glBegin(GL_LINES);
    for(unsigned int i = 0; i < edges.size(); i++){
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

void RenderObject::drawLines(const std::vector<glm::vec4> &vertices,
                             const Color &color) {

}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void RenderObject::setColor(Color color) {
    this->color = color;
}


void RenderObject::render(const mat4 &VP) {
    mat4 MVP = VP * getModelMatrix();
    const vector<vec4>& vertices = getVertices();

    unsigned int verticesCount = vertices.size();
    vector<vec4> transformedVertices(verticesCount);

    for(unsigned int i = 0; i < verticesCount; i++){
        transformedVertices[i] = MVP * vertices[i];

        transformedVertices[i].x /= transformedVertices[i].w;
        transformedVertices[i].y /= transformedVertices[i].w;
        transformedVertices[i].z /= transformedVertices[i].w;

    }
    setSurfaceColor(this->color);
    drawLines(transformedVertices);
}

void RenderObject::render(const glm::mat4 &VP, const Color &color) {
    mat4 MVP = VP * getModelMatrix();
    const vector<vec4>& vertices = getVertices();

    unsigned int verticesCount = vertices.size();
    vector<vec4> transformedVertices(verticesCount);

    for(unsigned int i = 0; i < verticesCount; i++){
        transformedVertices[i] = MVP * vertices[i];

        transformedVertices[i].x /= transformedVertices[i].w;
        transformedVertices[i].y /= transformedVertices[i].w;
        transformedVertices[i].z /= transformedVertices[i].w;

    }
    setSurfaceColor(color);
    drawLines(transformedVertices);
}