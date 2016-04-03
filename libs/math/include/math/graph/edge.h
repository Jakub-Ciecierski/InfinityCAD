//
// Created by jakub on 12/7/15.
//

#ifndef AC_EDGE_H
#define AC_EDGE_H


#include "node.h"

class Edge {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    unsigned int key;

    std::string color;
    std::string label;

    Node* node1;
    Node* node2;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    Edge(Node* node1, Node* node2, unsigned int key);

    std::string getColor();
    std::string getLabel();

    unsigned int getKey();

    Node* getNode1();
    Node* getNode2();

    void setColor(std::string color);
    void setLabel(std::string label);

    //-----------------------------------------------------------//
    //  OPERATORS
    //-----------------------------------------------------------//

    bool operator==(const Edge& edge) const;
};


#endif //AC_EDGE_H
