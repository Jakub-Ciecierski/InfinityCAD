//
// Created by jakub on 12/7/15.
//

#include <iostream>
#include "math/graph/graph.h"

Graph::Graph(){
    nodes = new std::vector<Node*>;
    edges = new std::vector<Edge*>;
}

Graph::~Graph(){
    for(unsigned int i = 0; i < nodes->size(); i++){
        delete (*nodes)[i];
    }

    for(unsigned int i = 0; i < edges->size(); i++){
        delete (*edges)[i];
    }

    delete nodes;
    delete edges;
}

//-----------------------------------------------------------//
//  GETTERS
//-----------------------------------------------------------//

const std::vector<Node*>* Graph::getNodes() const{
    return this->nodes;
}
const std::vector<Edge*>* Graph::getEdges() const{
    return this->edges;
}

//-----------------------------------------------------------//
//  SETTERS
//-----------------------------------------------------------//

void Graph::addNode(Node* node){
    // If node already exists, dont add it
    for(unsigned int i = 0; i < nodes->size(); i++){
        Node* currNode = (*nodes)[i];
        if (currNode == node)
            return;
    }

    nodes->push_back(node);
}

void Graph::addEdge(Edge* edge){
    for(unsigned int i = 0; i < edges->size(); i++){
        Edge* currEdge = (*edges)[i];
        if (currEdge == edge)
            return;
    }

    edges->push_back(edge);
}

void Graph::removeNode(unsigned int index){
    for(unsigned int i = 0; i < nodes->size(); i++){
        Node* currNode = (*nodes)[i];
        if (currNode->getKey() == index)
            nodes->erase(nodes->begin() + i);
    }
}
void Graph::removeEdge(unsigned int index){
    for(unsigned int i = 0; i < edges->size(); i++){
        Edge* currEdge = (*edges)[i];
        if (currEdge->getKey() == index)
            edges->erase(edges->begin() + i);
    }
}

void Graph::combineParallelEdges(){
    bool do_continue = true;

    while(do_continue) {
        do_continue = _findAndCombineParallelEdge();
    }
}

//-----------------------------------------------------------//
//  PRIVATE METHODS
//-----------------------------------------------------------//

bool Graph::_findAndCombineParallelEdge(){
    unsigned int edgeCount = this->edges->size();

    for (unsigned int i = 0; i < edgeCount - 1; i++) {
        Edge *edge1 = (*edges)[i];

        for (unsigned int j = i + 1; j < edgeCount; j++) {
            Edge *edge2 = (*edges)[j];

            Node *node1Edge1 = edge1->getNode1();
            Node *node2Edge1 = edge1->getNode2();

            Node *node1Edge2 = edge2->getNode1();
            Node *node2Edge2 = edge2->getNode2();

            if (*node1Edge1 == *node1Edge2 &&
                *node2Edge1 == *node2Edge2) {

                this->removeEdge(edge1->getKey());
                this->removeEdge(edge2->getKey());

                Edge *combinedEdge = new Edge(node1Edge1,
                                              node2Edge1,
                                              edge1->getKey());

                std::string combinedLabel = edge1->getLabel() + ", " +
                                            edge2->getLabel();

                combinedEdge->setColor(edge1->getColor());
                combinedEdge->setLabel(combinedLabel);

                this->addEdge(combinedEdge);

                return true;
            }

        }
    }
    return false;
}

//-----------------------------------------------------------//
//  OPERATORS
//-----------------------------------------------------------//

