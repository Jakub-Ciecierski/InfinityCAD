//
// Created by jakub on 4/2/16.
//

#include <infinity_cad/rendering/render_objects/primitivies/line.h>
#include <infinity_cad/rendering/color/color_settings.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Line::Line(SceneID id , glm::vec4 v1, glm::vec4 v2) :
        RenderObject(id),
        v1(v1), v2(v2){
    initVertices();
    initEdges();

    setColor(COLOR_OBJECT_DEFAULT);

    grabable = false;
}
Line::Line(SceneID id , std::string name,
        glm::vec4 v1, glm::vec4 v2) :
        RenderObject(id, name),
        v1(v1), v2(v2){
    initVertices();
    initEdges();

    setColor(COLOR_OBJECT_DEFAULT);

    grabable = false;
}

Line::Line(SceneID id , std::string name,
           glm::vec3 v1, glm::vec3 v2) :
        RenderObject(id, name){
    this->v1 = vec4(v1.x, v1.y, v1.z, 1.0f);
    this->v2 = vec4(v2.x, v2.y, v2.z, 1.0f);

    initVertices();
    initEdges();

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