//
// Created by jakub on 5/2/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Surface::Surface(SceneID id, std::string name,
                 int n, int m) :
        RenderObject(id, name), n(n), m(m){
    grabable = false;
}

Surface::~Surface(){}


//-----------------------//
//  PROTECTED
//-----------------------//

void Surface::initVertices(){

}

void Surface::initEdges() {

}

//-----------------------//
//  PUBLIC
//-----------------------//


void Surface::render(const glm::mat4 &VP) {
    draw(VP, color);
}
void Surface::render(const glm::mat4 &VP, const Color &color) {
    draw(VP, color);
}