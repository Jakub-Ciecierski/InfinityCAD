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
    NDCVertices.resize(vertices.size());

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

//-----------------------//
//  PUBLIC 
//-----------------------//

vec3 Cone::getClosestPoint(const vec3 point){
    return vec3(0,0,0);
}

float Cone::intersect(const RayCast & ray){
    return RAYCAST_NO_SOLUTION;
}
