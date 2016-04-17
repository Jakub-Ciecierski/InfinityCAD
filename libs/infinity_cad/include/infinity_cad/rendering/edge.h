//
// Created by jakub on 2/26/16.
//

#ifndef MG1_EDGE_H
#define MG1_EDGE_H

#include <glm/gtc/matrix_transform.hpp>
#include <infinity_cad/rendering/color/color.h>

/**
 * Edge class encapsulates an edge with two verticies.
 */
class Edge {
private:
    unsigned int i;
    unsigned int j;

public:
    Edge(unsigned int i, unsigned int j);

    const Color* color;

    bool costumWidth;
    float width;

    unsigned int getVertex1() const;
    unsigned int getVertex2() const;
};


#endif //MG1_LINE_H
