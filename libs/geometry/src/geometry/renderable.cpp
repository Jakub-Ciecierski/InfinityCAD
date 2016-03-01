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
    }

    const vector<Edge>& edges = getEdges();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    for(unsigned int i = 0; i < edges.size(); i++){
        unsigned int index1 = edges[i].getVertex1();
        unsigned int index2 = edges[i].getVertex2();

        glVertex3f(transformedVertices[index1].x,
                   transformedVertices[index1].y,
                   transformedVertices[index1].z);

        glVertex3f(transformedVertices[index2].x,
                   transformedVertices[index2].y,
                   transformedVertices[index2].z);
    }
    glEnd();
}