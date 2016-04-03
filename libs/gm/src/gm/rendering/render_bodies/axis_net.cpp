//
// Created by jakub on 3/27/16.
//

#include <gm/rendering/render_bodies/axis_net.h>
#include <gm/color/color_settings.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

AxisNet::AxisNet(int netSize) : netSize(netSize), middleLineColor(0,0,0,1) {
    middleLineWidth = 6.0f;
    setColor(COLOR_PLANE);

    initVertices();
    initEdges();
    NDCVertices.resize(vertices.size());

    grabable = false;
}

AxisNet::~AxisNet() {

}

//-----------------------//
//  PRIVATE
//-----------------------//

void AxisNet::initLine(glm::vec4 v1, glm::vec4 v2,
                       int& currentVertex,
                       const Color* color, float width){
    Line line(v1,v2);
    line.setColor(*color);
    line.setLineWidth(width);
    lines.push_back(line);
}

void AxisNet::initLineSegment(glm::vec4 v1, glm::vec4 v2,
                              int& currentVertex,
                              const Color* color, float width){
    initLine(v1, v2, currentVertex, color, width);

    int SEGMENTS = 1;
    float segDelta = 1 / (float)SEGMENTS;
    float currSeg = 0;

    while(currSeg <= 1){
        vec4 v = v1 + currSeg*(v2-v1);
        currSeg += segDelta;

        initLine(v1, v, currentVertex, color, width);
    }
}



void AxisNet::initXLines(float start, float finish,
                         float deltaDist, int& currentVertex){
    float xStart = 0;

    initLineSegment(vec4(xStart, 0, start, 1),
                    vec4(xStart, 0, finish, 1),
                    currentVertex, &middleLineColor, middleLineWidth);
    xStart += deltaDist;

    while(xStart <= finish){
        initLineSegment(vec4(xStart, 0, start, 1),
                        vec4(xStart, 0, finish, 1),
                        currentVertex, getColor());
        xStart += deltaDist;
    }
    initLineSegment(vec4(xStart, 0, start, 1),
                    vec4(xStart, 0, finish, 1),
                    currentVertex, getColor());

    xStart = -1;
    while(xStart <= 0){
        initLineSegment(vec4(xStart, 0, start, 1),
                        vec4(xStart, 0, finish, 1),
                        currentVertex, getColor());
        xStart += deltaDist;
    }
}

void AxisNet::initZLines(float start, float finish,
                         float deltaDist, int& currentVertex){
    float zStart = 0;

    initLineSegment(vec4(start, 0, zStart, 1),
             vec4(finish, 0, zStart, 1),
             currentVertex, &middleLineColor, middleLineWidth);
    zStart += deltaDist;

    while(zStart < finish){
        initLineSegment(vec4(start, 0, zStart, 1),
                 vec4(finish, 0, zStart, 1),
                 currentVertex, getColor());
        zStart += deltaDist;
    }
    initLineSegment(vec4(start, 0, zStart, 1),
                    vec4(finish, 0, zStart, 1),
                    currentVertex, getColor());

    zStart = -1;
    while(zStart < 0){
        initLineSegment(vec4(start, 0, zStart, 1),
                 vec4(finish, 0, zStart, 1),
                 currentVertex, getColor());
        zStart += deltaDist;
    }

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

    initXLines(start, finish, deltaDist, currentVertex);
    initZLines(start, finish, deltaDist, currentVertex);

}

void AxisNet::initEdges() {

}

//-----------------------//
//  PUBLIC
//-----------------------//


void AxisNet::render(const glm::mat4 &VP) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    for(auto& line : lines){
        line.render(VP);
    }

    glDisable(GL_BLEND);
}
void AxisNet::render(const glm::mat4 &VP, const Color &color) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    for(auto& line : lines){
        line.render(VP, color);
    }

    glDisable(GL_BLEND);
}

void AxisNet::update() {
    for(auto& line : lines){
        line.update();
    }
}


float AxisNet::intersect(const RayCast &ray) {
    return RAYCAST_NO_SOLUTION;
}

vec3 AxisNet::getClosestPoint(const vec3 point){
    return vec3(0,0,0);
}
