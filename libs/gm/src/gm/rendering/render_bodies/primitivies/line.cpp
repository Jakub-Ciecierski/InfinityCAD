//
// Created by jakub on 4/2/16.
//

#include <gm/rendering/render_bodies/primitivies/line.h>
#include <gm/color/color_settings.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Line::Line(vec4 v1, vec4 v2) :
    v1(v1), v2(v2){

    initVertices();
    initEdges();
    NDCVertices.resize(vertices.size());

    setColor(COLOR_OBJECT_DEFAULT);

    grabable = false;
}

Line::~Line(){

}



//-----------------------//
//  PRIVATE 
//-----------------------//

void Line::initVertices(){
    vertices.push_back(v1);
    vertices.push_back(v2);

    edges.push_back(Edge(0,1));
}

void Line::initEdges() {

}

//-----------------------//
//  PUBLIC
//-----------------------//

float Line::intersect(const RayCast &ray) {
    return RAYCAST_NO_SOLUTION;
}

vec3 Line::getClosestPoint(const vec3 point) {
    return vec3(0,0,0);
}
