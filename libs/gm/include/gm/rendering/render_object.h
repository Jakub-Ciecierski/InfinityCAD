//
// Created by jakub on 2/28/16.
//

#ifndef MG1_RENDERABLE_H
#define MG1_RENDERABLE_H

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <gm/object.h>
#include <gm/edge.h>
#include <gm/color/color.h>

/*
 * Renderable Interface.
 */
class RenderObject : public Object {
private:
    Color color;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    void setSurfaceColor(const Color& color);

    const std::vector<glm::vec4>& getVertices();
    const std::vector<Edge>& getEdges();

    /*
     * Draws lines after transformation of vertices.
     * Drawing is based on the Edges created in the initialization process
     */
    void drawLines(const std::vector<glm::vec4>& vertices);
    void drawLines(const std::vector<glm::vec4>& vertices,
                   const Color& color);

protected:

    std::vector<glm::vec4> vertices;
    std::vector<Edge> edges;

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    virtual void initVertices() = 0;
    virtual void initEdges() = 0;

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    RenderObject();

    virtual ~RenderObject();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void setColor(Color color);

    void render(const glm::mat4& VP);
    void render(const glm::mat4& VP, const Color& color);
};


#endif //MG1_RENDERABLE_H