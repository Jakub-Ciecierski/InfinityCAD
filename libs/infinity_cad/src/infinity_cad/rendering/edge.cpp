//
// Created by jakub on 2/26/16.
//

#include <infinity_cad/rendering/edge.h>

using namespace glm;

Edge::Edge(unsigned int i, unsigned int j) :
    i(i), j(j){
    color = NULL;

    costumWidth = false;
    width = 1.0f;
}

unsigned int Edge::getVertex1() const{
    return i;
}

unsigned int Edge::getVertex2() const{
    return j;
}
