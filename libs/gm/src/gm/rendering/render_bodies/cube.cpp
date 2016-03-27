//
// Created by jakub on 2/28/16.
//

#include "gm/rendering/ray_cast.h"
#include "gm/rendering/render_bodies/cube.h"

using namespace glm;
using namespace std;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

Cube::Cube() : RenderBody(){
    initVertices();
    initEdges();
    transformedVertices.resize(vertices.size());
}

Cube::~Cube() {

}

//-----------------------------------------------------------//
//  PROTECTED METHODS
//-----------------------------------------------------------//

void Cube::initVertices() {
    vertices.resize(8);
    vertices[0].x = -1;
    vertices[0].y = 1;
    vertices[0].z = -1;
    vertices[0].w = 1;

    vertices[1].x = 1;
    vertices[1].y = 1;
    vertices[1].z = -1;
    vertices[1].w = 1;

    vertices[2].x = 1;
    vertices[2].y = -1;
    vertices[2].z = -1;
    vertices[2].w = 1;

    vertices[3].x = -1;
    vertices[3].y = -1;
    vertices[3].z = -1;
    vertices[3].w = 1;

    vertices[4].x = -1;
    vertices[4].y = 1;
    vertices[4].z = 1;
    vertices[4].w = 1;

    vertices[5].x = 1;
    vertices[5].y = 1;
    vertices[5].z = 1;
    vertices[5].w = 1;

    vertices[6].x = 1;
    vertices[6].y = -1;
    vertices[6].z = 1;
    vertices[6].w = 1;

    vertices[7].x = -1;
    vertices[7].y = -1;
    vertices[7].z = 1;
    vertices[7].w = 1;
}

void Cube::initEdges() {
    edges.push_back(Edge(0,1));
    edges.push_back(Edge(1,2));
    edges.push_back(Edge(2,3));
    edges.push_back(Edge(3,0));

    edges.push_back(Edge(4,5));
    edges.push_back(Edge(5,6));
    edges.push_back(Edge(6,7));
    edges.push_back(Edge(7,4));

    edges.push_back(Edge(0,4));
    edges.push_back(Edge(1,5));
    edges.push_back(Edge(2,6));
    edges.push_back(Edge(3,7));
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

float Cube::intersect(const RayCast& ray){
    return RAYCAST_NO_SOLUTION;
}