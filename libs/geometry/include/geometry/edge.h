//
// Created by jakub on 2/26/16.
//

#ifndef MG1_LINE_H
#define MG1_LINE_H

#include <glm/gtc/matrix_transform.hpp>

/**
 * Edge class encapsulates an edge with two verticies.
 */
class Edge {
private:
    glm::fvec4* vertex1;
    glm::fvec4* vertex2;

    unsigned int i;
    unsigned int j;
public:

    Edge(glm::fvec4* vertex1, glm::fvec4* vertex2);

    Edge(unsigned int i, unsigned int j);

    unsigned int getVertex1() const;
    unsigned int getVertex2() const;
};


#endif //MG1_LINE_H
