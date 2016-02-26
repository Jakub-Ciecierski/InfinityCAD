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
public:

    Edge(glm::fvec4* vertex1, glm::fvec4* vertex2);
};


#endif //MG1_LINE_H
