//
// Created by jakub on 2/28/16.
//

#ifndef MG1_RENDERABLE_H
#define MG1_RENDERABLE_H

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <gm/edge.h>
#include <gm/color/color.h>
#include <gm/rigid_body.h>
#include "gm/rendering/ray_cast.h"


/*
 * Renderable Interface.
 */
class RenderBody : public RigidBody {
private:
    Color color;
    //Color selectedColor;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    void setSurfaceColor(const Color& color);

protected:
    float lineWidth;

    std::vector<glm::vec4> vertices;
    std::vector<Edge> edges;

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//
    const std::vector<glm::vec4>& getVertices();
    const std::vector<Edge>& getEdges();

    virtual void initVertices() = 0;
    virtual void initEdges() = 0;

    /*
     * Draws lines after transformation of vertices.
     * Drawing is based on the Edges created in the initialization process
     */
    virtual void drawLines(const std::vector<glm::vec4>& vertices,
                           bool costumColor);

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    RenderBody();

    virtual ~RenderBody();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//
    glm::vec2 transformed;

    void setColor(Color color);
    const Color* getColor();

    virtual float intersect(const RayCast& ray) = 0;

    void render(const glm::mat4& VP);
    void render(const glm::mat4& VP, const Color& color);
};


#endif //MG1_RENDERABLE_H