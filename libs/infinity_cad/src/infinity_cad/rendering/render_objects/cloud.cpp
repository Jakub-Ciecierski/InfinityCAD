//
// Created by jakub on 3/29/16.
//

#include <infinity_cad/rendering/render_objects/cloud.h>


using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Cloud::Cloud(SceneID id, std::vector<glm::vec4>& points) :
        RenderObject(id){
    vertices = points;
    initVertices();
    initEdges();

    worldVertices.resize(vertices.size());
    NDCVertices.resize(vertices.size());
}
Cloud::Cloud(SceneID id, std::string name,
             std::vector<glm::vec4>& points) :
        RenderObject(id, name){
    vertices = points;
    initVertices();
    initEdges();

    worldVertices.resize(vertices.size());
    NDCVertices.resize(vertices.size());
}

Cloud::~Cloud() {

}


//-----------------------//
//  PROTECTED
//-----------------------//


void Cloud::initVertices(){

}

void Cloud::initEdges() {

}

void Cloud::drawLines(const std::vector<glm::vec4> &vertices,
                      bool costumColor) {
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for(unsigned int i = 0; i < vertices.size(); i++){
        if(vertices[i].w < 0) continue;

        glVertex2f(vertices[i].x,
                   vertices[i].y);
    }
    glEnd();
}

//-----------------------//
//  PUBLIC
//-----------------------//

const std::vector<glm::vec4>& Cloud::getWorldVertices(){
    return this->worldVertices;
}

void Cloud::transform(const glm::mat4 &VP) {
    const mat4& ModelMatrix = getModelMatrix();
    mat4 MVP = VP * ModelMatrix;
    transformPosition(VP);

    for(unsigned int i = 0; i < NDCVertices.size(); i++){
        worldVertices[i] = ModelMatrix * vertices[i];

        NDCVertices[i] = MVP * vertices[i];

        NDCVertices[i].x /= NDCVertices[i].w;
        NDCVertices[i].y /= NDCVertices[i].w;
        NDCVertices[i].z /= NDCVertices[i].w;

    }
}