//
// Created by jakub on 2/28/16.
//

#ifndef MG1_RENDERABLE_H
#define MG1_RENDERABLE_H

#include <infinity_cad/rigid_object.h> // must be before ray_Cast.h
#include <infinity_cad/rendering/color/color.h>
#include "infinity_cad/rendering/edge.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>

#include <vector>


class RayCast;

/*
 * Renderable Interface.
 */
class RenderObject : public RigidObject {
private:

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//
    bool doRender;

protected:
    Color color;

    GLenum drawingMode;

    bool grabable;
    float lineWidth;

    bool selected;

    std::vector<glm::vec4> vertices;
    std::vector<Edge> edges;

    std::vector<glm::vec4> NDCVertices;
    glm::vec3 NDCPosition;

    std::vector<RenderObject *> children;

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    void transformPosition(const glm::mat4 &VP);
    virtual void transform(const glm::mat4 &VP);

    void setSurfaceColor(const Color& color);

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

    RenderObject(SceneID id);
    RenderObject(SceneID id, std::string name);

    virtual ~RenderObject();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    void setShow(bool value);

    void addChild(RenderObject * body);

    virtual const std::vector<RenderObject *>& getChildren();

    void setDrawingMode(GLenum mode);
    void setLineWidth(float width);

    bool isGrabable();

    void setColor(Color color);
    const Color* getColor();

    const glm::vec3& getProjectedPosition();

    void setSelected(bool v);
    bool isSelected();

    virtual float intersect(const RayCast& ray);

    virtual void render(const glm::mat4& VP);
    virtual void render(const glm::mat4& VP, const Color& color);
};


#endif //MG1_RENDERABLE_H