//
// Created by jakub on 2/28/16.
//


#include <gm/color/color_settings.h>
#include <gm/rendering/render_body.h>
#include <iostream>
#include <gm/rendering/clipping/sutherland.h>
#include <gm/util/utils.h>

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

        float near = 0.1f;

        bool clipped = false;


        /*
        if(-v1.w > v1.x > v1.w ||
                -v1.w > v1.y > v1.w ||
                -v1.w > v1.z > v1.w ) {
            std::cout << "V1 Clipped" << std::endl;

            //float x_c = (v1.x - v1.w) / ((v));
            clipped = true;
            continue;
        }

        if(-v2.w > v2.x > v2.w ||
           -v2.w > v2.y > v2.w ||
           -v2.w > v2.z > v2.w ) {
            std::cout << "V2 Clipped" << std::endl;
            clipped = true;
            continue;
        }
        if(!clipped){
            std::cout << "Nothing Clipped" << std::endl;
        }
*/

        // CLIPPING
        // Don't draw if either is not visiable
/*
        if(v1.w < 0 || v2.w < 0 ){

            continue;
        }*/

        /*
        if(v1.w < 0 && v2.w < 0 ) {
            //float t = (v1.x - v1.w) / ((v2.w - v1.w) - (v2.x - v1.x));

            std::cout << v1.x << ", " << v1.y
            << ", " << v1.z  << ", " << v1.w << std::endl;
            continue;
        }

        if(v1.w < 0) {
            //float t = (v1.x - v1.w) / ((v2.w - v1.w) - (v2.x - v1.x));
            float t = - (v1.w / (v2.w - v1.w));

            v1 = v1 + t*(v2-v1);
        }
        if(v2.w < 0) {
            float t = - (v1.w / (v2.w - v1.w));
            continue;
            v2 = v1 + t*(v2-v1);
        }*/

        //std::cout << "..." << std::endl;
        /*
        if(v1.z < 0 || v2.z < 0) {
            //std::cout << v1.w << "," << v2.w << std::endl;
            continue;
        }

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
*/
        /*
        bool doDraw = clip(v1, v2,
                           -1, 1,
                           -1, 1);
*/
        bool doDraw = clip(v1, v2,
                           -1, 1,
                           -1, 1);
        if(doDraw) {
            /*
            if(((v1.x < -1.0 || v1.x > 1.0) || (v2.x < -1.0 || v2.x > 1.0)) ||
            ((v1.y < -1.0 || v1.y > 1.0) || (v2.y < -1.0 || v2.y > 1.0)))
                continue;*/
            glVertex2f(v1.x,
                       v1.y);

            glVertex2f(v2.x,
                       v2.y);
            //std::cout << "DRAWING" << std::endl;
        }

    }
    glEnd();
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void RenderBody::setDrawingMode(GLenum mode){
    this->drawingMode = mode;
}

void RenderBody::setLineWidth(float width){
    this->lineWidth = width;
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

