//
// Created by jakub on 3/27/16.
//

#include <gm/rendering/render_bodies/axis_net.h>
#include <gm/color/color_settings.h>
#include <GL/gl.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

AxisNet::AxisNet(int netSize) : netSize(netSize) {
    initVertices();
    initEdges();
    transformedVertices.resize(vertices.size());

    setColor(COLOR_PLANE);

    grabable = false;
}

AxisNet::~AxisNet() {

}

//-----------------------//
//  PROTECTED
//-----------------------//

void AxisNet::initVertices(){
    float start = -1.0f;
    float finish = 1.0f;

    float screenWidth = abs(start) + abs(finish);
    float deltaDist = screenWidth/(float)netSize;

    int currentVertex = 0;
    float xStart = start;
    while(xStart < finish){
        vec4 line1(xStart, 0, start, 1);
        vec4 line2(xStart, 0, finish, 1);

        vertices.push_back(line1);
        vertices.push_back(line2);

        edges.push_back(Edge(currentVertex, currentVertex+1));
        xStart += deltaDist;
        currentVertex += 2;
    }

    float zStart = start;
    while(zStart < finish){
        vec4 line1(start, 0, zStart, 1);
        vec4 line2(finish, 0, zStart, 1);

        vertices.push_back(line1);
        vertices.push_back(line2);

        edges.push_back(Edge(currentVertex, currentVertex+1));
        zStart += deltaDist;
        currentVertex += 2;
    }

}

void AxisNet::initEdges() {

}


void AxisNet::drawLines(const std::vector<glm::vec4> &vertices,
                        bool costumColor) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    RenderBody::drawLines(vertices, costumColor);

    glDisable(GL_BLEND);
}


//-----------------------//
//  PUBLIC
//-----------------------//


float AxisNet::intersect(const RayCast &ray) {
    return RAYCAST_NO_SOLUTION;
}