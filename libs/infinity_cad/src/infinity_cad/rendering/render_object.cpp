//
// Created by jakub on 2/28/16.
//

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/color/color_settings.h>
#include <infinity_cad/rendering/visibility/ray_cast.h>
#include <infinity_cad/rendering/clipping/sutherland.h>

#include <stdexcept>
#include <ifc_gpu/projection/ndc_gpu.h>
#include <iostream>
#include <infinity_cad/settings/settings.h>


using namespace glm;
using namespace std;

//-----------------------------------------------------------//
//  CONSTRUCTORS
//-----------------------------------------------------------//

RenderObject::RenderObject(SceneID id) :
    RigidObject(id),
    color(COLOR_OBJECT_DEFAULT){
    lineWidth = 1.0f;

    grabable = true;
    doRender = true;

    drawingMode = GL_LINES;

    setSelected(false);

    NDCVertices = NULL;
}
RenderObject::RenderObject(SceneID id, std::string name) :
        RigidObject(id, name),
        color(COLOR_OBJECT_DEFAULT){
    lineWidth = 1.0f;

    grabable = true;
    doRender = true;

    drawingMode = GL_LINES;

    setSelected(false);

    NDCVertices = NULL;
}

RenderObject::~RenderObject() {
    for(unsigned int i = 0;i < children.size(); i++){
        delete children[i];
    }
    if(NDCVertices != NULL) delete NDCVertices;
}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

void RenderObject::setSurfaceColor(const Color &color) {
    glColor4f(color.R, color.G, color.B, color.Alpha);
}

const vector<vec4>&RenderObject::getVertices() {
    return this->vertices;
}

const vector<Edge>&RenderObject::getEdges() {
    return this->edges;
}

void RenderObject::transformPosition(const glm::mat4 &VP){
    vec4 transformedOrigins(getPosition(), 1);

    transformedOrigins = VP * transformedOrigins;
    transformedOrigins.x /= transformedOrigins.w;
    transformedOrigins.y /= transformedOrigins.w;
    transformedOrigins.z /= transformedOrigins.w;

    NDC_W = transformedOrigins.w;

    NDCPosition.x = transformedOrigins.x;
    NDCPosition.y = transformedOrigins.y;
    NDCPosition.z = transformedOrigins.z;
}

void RenderObject::transform(const mat4 &VP){
    mat4 MVP = VP * getModelMatrix();
    transformPosition(VP);

    unsigned int verticesSize = vertices.size();
    if (NDCVertices == NULL)
        NDCVertices = (vec4 *) malloc(vertices.size() * sizeof(vec4));

    if(verticesSize > ifc::VERTEX_LOWER_BOUND_CUDA && ifc::RUN_CUDA) {
        ndc_compute(vertices.data(), vertices.size(), &MVP, &NDCVertices);
    }else{
        for(unsigned int i = 0; i < verticesSize; i++){
            NDCVertices[i] = MVP * vertices[i];

            NDCVertices[i].x /= NDCVertices[i].w;
            NDCVertices[i].y /= NDCVertices[i].w;
            NDCVertices[i].z /= NDCVertices[i].w;
        }
    }
}

void RenderObject::drawLines(bool costumColor){
    const vector<Edge>& edges = getEdges();

    glLineWidth((GLfloat)lineWidth);
    glBegin(drawingMode);
    for(unsigned int i = 0; i < edges.size(); i++){
        if(costumColor && edges[i].color != NULL)
            setSurfaceColor(*(edges[i].color));

        unsigned int index1 = edges[i].getVertex1();
        unsigned int index2 = edges[i].getVertex2();

        vec4* v1 = &(NDCVertices[index1]);
        vec4* v2 = &(NDCVertices[index2]);

        bool doDraw = clip(*v1, *v2, -1, 1, -1, 1);
        if(doDraw) {
            glVertex2f(v1->x, v1->y);
            glVertex2f(v2->x, v2->y);
        }
    }
    glEnd();
}

//-----------------------------------------------------------//
//  PUBLIC METHODS
//-----------------------------------------------------------//

void RenderObject::setShow(bool value){
    doRender = value;
}

void RenderObject::addChild(RenderObject * body){
    return this->children.push_back(body);
}

const std::vector<RenderObject *>& RenderObject::getChildren(){
    return this->children;
}

const std::vector<RenderObject *>& RenderObject::getComponents(){
    return this->components;
}

void RenderObject::setDrawingMode(GLenum mode){
    this->drawingMode = mode;
}

void RenderObject::setLineWidth(float width){
    this->lineWidth = width;
}

bool RenderObject::isGrabable() {
    if(!doRender) return false;
    return grabable;
}

void RenderObject::setColor(Color color) {
    this->color = color;
}

const vec3&RenderObject::getProjectedPosition(){
    return this->NDCPosition;
}

const Color*RenderObject::getColor() {
    return &color;
}
void RenderObject::setSelected(bool v){
    selected = v;
}

bool RenderObject::isSelected(){
    return selected;
}


float RenderObject::intersect(const RayCast &ray){
    return RAYCAST_NO_SOLUTION;
}

void RenderObject::render(const mat4 &VP) {
    transform(VP);
    if(doRender) {
        setSurfaceColor(this->color);
        drawLines(true);
    }
}

void RenderObject::render(const glm::mat4 &VP, const Color &color) {
    transform(VP);
    if(doRender) {
        setSurfaceColor(color);
        drawLines(false);
    }
}


