//
// Created by jakub on 3/23/16.
//

#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/util/utils.h>

using namespace ic;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Point::Point(SceneID id) :
        RenderBody(id){
    radius = 0.01;
    toleratedRadius = 0.1;

    initVertices();
    initEdges();
    NDCVertices.resize(vertices.size());

    drawingMode = GL_TRIANGLE_FAN;
}
Point::Point(SceneID id, std::string name) :
        RenderBody(id, name){
    radius = 0.01;
    toleratedRadius = 0.1;

    initVertices();
    initEdges();
    NDCVertices.resize(vertices.size());

    drawingMode = GL_TRIANGLE_FAN;
}


Point::~Point() {

}

void Point::initVertices() {
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

    vec3 scaleVec(radius, radius, radius);
    for(int i = 0; i < 8; i++){
        vertices[i] =
                glm::scale(glm::mat4(1.0f), scaleVec) * vertices[i];
    }
}

void Point::initEdges() {
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

float Point::intersect(const RayCast& ray){
    int solutions = 0;
    const vec3& origin = ray.getOrigin();
    const vec3& direction = ray.getDirection();
    const vec3& center = this->getPosition();
    vec3 tmp = (origin - center);

    float b = gm::dot(direction, tmp);
    float c = gm::dot(tmp, tmp) - (toleratedRadius * toleratedRadius);

    float delta = (b*b - c);
    if(isnan(delta) == 1 || delta < 0) return RAYCAST_NO_SOLUTION;
    else if(delta > 0) solutions = 2;
    else solutions = 1;

    float deltaSqrt = sqrt(delta);

    float t1 = -b + deltaSqrt;
    float t2 = -b - deltaSqrt;
    float t;
    if(solutions == 2){
        t = t1 < t2 ? t1 : t2;
    }else{
        t = t1;
        if(isnan(t) == 1) t = t2;
    }

    return t;
}

vec3 Point::getClosestPoint(const vec3 point){
    return vec3(0,0,0);
}
