//
// Created by jakub on 12/7/15.
//

#ifndef AC_GRAPH_H
#define AC_GRAPH_H

#include "node.h"
#include "edge.h"
#include <vector>

class Graph {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    std::vector<Node*>* nodes;
    std::vector<Edge*>* edges;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    /*
     * Finds and Combines a first pair of parallel edges that it finds.
     * Returns true if such pair was found, false otherwise
     */
    bool _findAndCombineParallelEdge();

public:
    Graph();

    ~Graph();

    //-----------------------------------------------------------//
    //  GETTERS
    //-----------------------------------------------------------//

    const std::vector<Node*>* getNodes() const;
    const std::vector<Edge*>* getEdges() const;

    //-----------------------------------------------------------//
    //  SETTERS
    //-----------------------------------------------------------//

    void addNode(Node* node);
    void addEdge(Edge* edge);

    void removeNode(unsigned int index);
    void removeEdge(unsigned int index);

    /*
     * Combines all edges with the same connection into one,
     * Combining all labels into one
     */
    void combineParallelEdges();
};


#endif //AC_GRAPH_H
