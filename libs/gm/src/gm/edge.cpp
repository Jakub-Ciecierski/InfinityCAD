//
// Created by jakub on 2/26/16.
//

#include <gm/edge.h>

using namespace glm;

Edge::Edge(unsigned int i, unsigned int j) :
    i(i), j(j){

}

unsigned int Edge::getVertex1() const{
    return i;
}

unsigned int Edge::getVertex2() const{
    return j;
}
