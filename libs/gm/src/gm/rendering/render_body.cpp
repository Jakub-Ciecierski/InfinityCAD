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

RenderBody::RenderBody(SceneID id) :
    RigidBody(id),
    color(COLOR_OBJECT_DEFAULT){
    lineWidth = 1.0f;

    grabable = true;
    doRender = true;

    drawingMode = GL_LINES;
}
RenderBody::RenderBody(SceneID id, std::string name) :
        RigidBody(id, name),
        color(COLOR_OBJECT_DEFAULT){
    lineWidth = 1.0f;

    grabable = true;
    doRender = true;

    drawingMode = GL_LINES;
}

RenderBody::~RenderBody() {
    for(unsigned int i = 0;i < children.size(); i++){
        delete children[i];
    }
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

void RenderBody::setShow(bool value){
    doRender = value;
}

void RenderBody::addChild(RenderBody* body){
    return this->children.push_back(body);
}

const std::vector<RenderBody*>& RenderBody::getChildren(){
    return this->children;
}

void RenderBody::setDrawingMode(GLenum mode){
    this->drawingMode = mode;
}

void RenderBody::setLineWidth(float width){
    this->lineWidth = width;
}

bool RenderBody::isGrabable() {
    if(!doRender) return false;
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
    if(doRender) {
        setSurfaceColor(this->color);
        drawLines(NDCVertices, true);
    }
}

void RenderBody::render(const glm::mat4 &VP, const Color &color) {
    transform(VP);
    if(doRender) {
        setSurfaceColor(color);
        drawLines(NDCVertices, false);
    }
}

