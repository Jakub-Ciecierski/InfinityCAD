//
// Created by jakub on 4/30/16.
//

#include <glm/detail/type_mat.hpp>
#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/render_objects/selection_box.h>
#include <infinity_cad/rendering/color/color_settings.h>
#include <infinity_cad/math/math.h>
#include <infinity_cad/rendering/visibility/ray_cast.h>
#include <algorithm>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

SelectionBox::SelectionBox(SceneID id, std::string name,
                           const glm::vec2& startPos) :
        RenderObject(id, name){
    this->startPos = startPos;
    this->color = COLOR_SELECTION_BOX_BORDER;
}

SelectionBox::~SelectionBox(){}

//-----------------------//
//  PROTECTED
//-----------------------//

void SelectionBox::initVertices() {

}

void SelectionBox::initEdges() {

}

//-----------------------//
//  PUBLIC
//-----------------------//

std::vector<RenderObject*> SelectionBox::scanBox(const Cross* cross,
                                                 RayCast* ray,
                                                 int windowWidth,
                                                 int windowHeight){
    int startX, startY;
    int endX, endY;
    std::vector<RenderObject*> objects;

    startX = ifc::xGLToPixelCoord(startPos.x, windowWidth);
    startY = ifc::yGLToPixelCoord(startPos.y, windowHeight);

    endX = ifc::xGLToPixelCoord(endPos.x, windowWidth);
    endY = ifc::yGLToPixelCoord(endPos.y, windowHeight);

    if(startX > endX){
        int tmp = startX;
        startX = endX;
        endX = tmp;
    }
    if(startY > endY){
        int tmp = startY;
        startY = endY;
        endY = tmp;
    }

    for(int x = startX; x < endX; x++){
        for(int y = startY; y < endY; y++){
            float xGL = ifc::xPixelToGLCoord(x, windowWidth);
            float yGL = ifc::yPixelToGLCoord(y, windowHeight);
            ray->update(xGL, yGL);

            std::vector<RenderObject*> closestBodies
                    = cross->getClosestObjectVector(*ray,
                                                    windowWidth, windowHeight,
                                                    10);
            for(unsigned int i = 0; i < closestBodies.size(); i++){
                RenderObject* closestBody = closestBodies[i];
                if(closestBody != NULL){
                    if (!(std::find(objects.begin(), objects.end(), closestBody)
                          != objects.end())) {
                        objects.push_back(closestBody);
                    }
                }
            }

        }
    }
    return objects;
}

void SelectionBox::updateEndPosition(const glm::vec2& endPos){
    this->endPos = endPos;
}

void SelectionBox::render(const glm::mat4 & VP){
    vec2 v1(endPos.x, startPos.y);
    vec2 v2(startPos.x, endPos.y);

    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);

    glBegin(GL_LINES);

    glVertex2f(startPos.x, startPos.y);
    glVertex2f(v1.x, v1.y);

    glVertex2f(v1.x, v1.y);
    glVertex2f(endPos.x, endPos.y);

    glVertex2f(endPos.x, endPos.y);
    glVertex2f(v2.x, v2.y);

    glVertex2f(v2.x, v2.y);
    glVertex2f(startPos.x, startPos.y);

    glEnd();

    setSurfaceColor(COLOR_SELECTION_BOX_FILL);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(startPos.x, startPos.y);
    glVertex2f(v1.x, v1.y);

    glVertex2f(v1.x, v1.y);
    glVertex2f(endPos.x, endPos.y);

    glVertex2f(endPos.x, endPos.y);
    glVertex2f(v2.x, v2.y);

    glVertex2f(v2.x, v2.y);
    glVertex2f(startPos.x, startPos.y);

    glEnd();
}

void SelectionBox::render(const glm::mat4 &VP, const Color& color) {
    vec2 v1(endPos.x, startPos.y);
    vec2 v2(startPos.x, endPos.y);

    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);

    glBegin(GL_LINES);

    glVertex2f(startPos.x, startPos.y);
    glVertex2f(v1.x, v1.y);

    glVertex2f(v1.x, v1.y);
    glVertex2f(endPos.x, endPos.y);

    glVertex2f(endPos.x, endPos.y);
    glVertex2f(v2.x, v2.y);

    glVertex2f(v2.x, v2.y);
    glVertex2f(startPos.x, startPos.y);

    glEnd();

    setSurfaceColor(COLOR_SELECTION_BOX_FILL);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(startPos.x, startPos.y);
    glVertex2f(v1.x, v1.y);

    glVertex2f(v1.x, v1.y);
    glVertex2f(endPos.x, endPos.y);

    glVertex2f(endPos.x, endPos.y);
    glVertex2f(v2.x, v2.y);

    glVertex2f(v2.x, v2.y);
    glVertex2f(startPos.x, startPos.y);

    glEnd();
}
