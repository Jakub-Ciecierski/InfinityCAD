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
}
Cloud::Cloud(SceneID id, std::string name,
             std::vector<glm::vec4>& points) :
        RenderObject(id, name){
    vertices = points;
    initVertices();
    initEdges();

    worldVertices.resize(vertices.size());
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

void Cloud::drawLines(bool costumColor) {
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for(unsigned int i = 0; i < vertices.size(); i++){
        if(NDCVertices[i].w < 0) continue;

        glVertex2f(NDCVertices[i].x, NDCVertices[i].y);
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

    unsigned int verticesSize = vertices.size();
    if (NDCVertices == NULL)
        NDCVertices = (vec4 *) malloc(vertices.size() * sizeof(vec4));

    for(unsigned int i = 0; i < verticesSize ; i++){
        worldVertices[i] = ModelMatrix * vertices[i];

        NDCVertices[i] = MVP * vertices[i];

        NDCVertices[i].x /= NDCVertices[i].w;
        NDCVertices[i].y /= NDCVertices[i].w;
        NDCVertices[i].z /= NDCVertices[i].w;

    }
}