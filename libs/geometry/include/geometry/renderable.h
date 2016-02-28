//
// Created by jakub on 2/28/16.
//

#ifndef MG1_RENDERABLE_H
#define MG1_RENDERABLE_H

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "edge.h"
#include "object.h"

/*
 * Renderable Interface.
 */
class Renderable : public Object {
private:
    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    const std::vector<glm::vec4>& getVertices();
    const std::vector<Edge>& getEdges();

    void init();

protected:

    std::vector<glm::vec4> vertices;
    std::vector<Edge> edges;
    int verticesGenerated;

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    virtual void initVertices() = 0;
    virtual void initEdges() = 0;

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Renderable();

    virtual ~Renderable();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void render(const glm::mat4& VP);
};


#endif //MG1_RENDERABLE_H