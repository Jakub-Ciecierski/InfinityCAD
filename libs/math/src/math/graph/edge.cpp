//
// Created by jakub on 12/7/15.
//

#include "math/graph/edge.h"

Edge::Edge(Node* node1, Node* node2, unsigned int key){
    this->node1 = node1;
    this->node2 = node2;

    this->key = key;

    color = "";
    label = "";
}

std::string Edge::getColor(){
    return this->color;
}

std::string Edge::getLabel(){
    return this->label;
}

unsigned int Edge::getKey(){
    return this->key;
}

Node* Edge::getNode1(){
    return this->node1;
}
Node* Edge::getNode2(){
    return this->node2;
}

void Edge::setColor(std::string color){
    this->color = color;
}
void Edge::setLabel(std::string label){
    this->label = label;
}

//-----------------------------------------------------------//
//  OPERATORS
//-----------------------------------------------------------//


bool Edge::operator==(const Edge& edge) const{
    return (this->key == edge.key);
}