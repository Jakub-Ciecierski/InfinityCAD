//
// Created by jakub on 2/28/16.
//


#include <gm/color/color_settings.h>
#include <gm/rendering/render_body.h>
#include <iostream>

using namespace glm;
using namespace std;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

RenderBody::RenderBody() : color(COLOR_OBJECT_DEFAULT){
    lineWidth = 1.0f;
    grabable = true;

    drawingMode = GL_LINES;
}

RenderBody::~RenderBody() {

}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

void RenderBody::setSurfaceColor(const Color &color) {
    glColor3f(color.R, color.G, color.B);
}

const vector<vec4>& RenderBody::getVertices() {
    return this->vertices;
}

const vector<Edge>& RenderBody::getEdges() {
    return this->edges;
}

void RenderBody::transformPosition(const glm::mat4 &VP){
    vec4 transformedOrigins(getPosition(), 1);

    transformedOrigins = VP * transformedOrigins;
    transformedOrigins.x /= transformedOrigins.w;
    transformedOrigins.y /= transformedOrigins.w;
    transformedOrigins.z /= transformedOrigins.w;

    NDCPosition.x = transformedOrigins.x;
    NDCPosition.y = transformedOrigins.y;
    NDCPosition.z = transformedOrigins.z;
}

void RenderBody::transform(const mat4 &VP){
    mat4 MVP = VP * getModelMatrix();
    transformPosition(VP);

    for(unsigned int i = 0; i < NDCVertices.size(); i++){
        NDCVertices[i] = MVP * vertices[i];

        NDCVertices[i].x /= NDCVertices[i].w;
        NDCVertices[i].y /= NDCVertices[i].w;
        NDCVertices[i].z /= NDCVertices[i].w;

    }
}

void RenderBody::drawLines(const std::vector<glm::vec4>& vertices,
                           bool costumColor){
    const vector<Edge>& edges = getEdges();

    glLineWidth((GLfloat)lineWidth);

    glBegin(drawingMode);
    for(unsigned int i = 0; i < edges.size(); i++){
        if(costumColor && edges[i].color != NULL)
            setSurfaceColor(*(edges[i].color));

        unsigned int index1 = edges[i].getVertex1();
        unsigned int index2 = edges[i].getVertex2();
        vec4 v1 = vertices[index1];
        vec4 v2 = vertices[index2];

        float near = 0.0f;

        // CLIPPING
        // Don't draw if either is not visiable
        if(v1.w < near || v2.w < near ) continue;

        // v2 is behind
        if(v2.w < near){
            float n = (v1.w - near) / (v1.w - v2.w);
            float xc = (n * v1.x) + ((1-n) * v2.x);
            float yc = (n * v1.y) + ((1-n) * v2.y);

            v2 = vec4(xc,yc,0,v2.w);
        }
        // v1 is behind
        else if(v1.w < near){
            float n = (v2.w - near) / (v2.w - v1.w);
            float xc = (n * v2.x) + ((1-n) * v1.x);
            float yc = (n * v2.y) + ((1-n) * v1.y);

            v1 = vec4(xc,yc,0,v1.w);
        }


        glVertex2f(v1.x,
                   v1.y);
        glVertex2f(v2.x,
                   v2.y);
    }
    glEnd();
}

char RenderBody::ComputeOutcode(vec4 p, int left, int right, int top, int bottom)
{
    char outcode = 0;
    if (p.x > right) outcode |= 2;
    else if (p.x < left) outcode |= 1;
    if (p.y < top) outcode |= 8;
    else if (p.y > bottom) outcode |= 4;
    return outcode;
}

bool RenderBody::CohenSutherland(vec4& p1, vec4& p2,
                     int left, int right,
                     int top, int bottom) {
    bool accept = false, done = false;
    char outcode1 = ComputeOutcode(p1, left,right,top,bottom);
    char outcode2 = ComputeOutcode(p2, left,right,top,bottom);
    do {
        if ( ( outcode1 | outcode2 ) == 0 ) { //trivially accepted
            accept = true;
            done = true;
        }
        else if ( ( outcode1 & outcode2 ) != 0 ) { //trivially rejected
            accept = false;
            done = true;
        }
        else { //subdivide
            char outcodeOut = (outcode1 != 0) ? outcode1 : outcode2;
            vec4 p;
            if ( ( outcodeOut & 8 ) != 0 ) {
                p.x = p1.x + (p2.x - p1.x)*(top - p1.y)/(p2.y - p1.y);
                p.y = top;
            }
            else if ( ( outcodeOut & 4 ) != 0 ) {
                p.x = p1.x + (p2.x - p1.x)*(bottom - p1.y)/(p2.y - p1.y);
                p.y = bottom;
            }
            else if ((outcodeOut & 2) != 0)
            {
                p.x = right;
                p.y = ((p2.y-p1.y)/(p2.x-p1.x))*(right-p1.x)+p1.y;
            }
            else if ((outcodeOut & 1) != 0)
            {
                p.x = left;
                p.y = ((p2.y - p1.y) / (p2.x - p1.x)) * (left - p1.x) + p1.y;
            }
            if (outcodeOut == outcode1){
                p1 = p;
                outcode1 = ComputeOutcode(p1, left,right,top,bottom);
            }
            else {
                p2 = p;
                outcode2 = ComputeOutcode(p2, left,right,top,bottom);
            }
        }
    } while (!done);
    return accept;
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void RenderBody::setDrawingMode(GLenum mode){
    this->drawingMode = mode;
}

bool RenderBody::isGrabable() {
    return grabable;
}

void RenderBody::setColor(Color color) {
    this->color = color;
}

const vec3& RenderBody::getProjectedPosition(){
    return this->NDCPosition;
}

const Color* RenderBody::getColor() {
    return &color;
}

void RenderBody::render(const mat4 &VP) {
    transform(VP);

    setSurfaceColor(this->color);
    drawLines(NDCVertices, true);
}

void RenderBody::render(const glm::mat4 &VP, const Color &color) {
    transform(VP);

    setSurfaceColor(color);
    drawLines(NDCVertices, false);
}

