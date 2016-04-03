//
// Created by jakub on 12/7/15.
//

#ifndef AC_NODE_H
#define AC_NODE_H

#include <string>

class Node {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    unsigned int key;

    std::string color;
    std::string label;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    Node(unsigned int key);

    std::string getColor();
    std::string getLabel();

    unsigned int getKey();

    void setColor(std::string color);
    void setLabel(std::string label);


    //-----------------------------------------------------------//
    //  OPERATORS
    //-----------------------------------------------------------//

    bool operator==(const Node& node) const;
};


#endif //AC_NODE_H
