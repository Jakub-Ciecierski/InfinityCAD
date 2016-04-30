//
// Created by jakub on 4/30/16.
//

#ifndef IC_SELECTION_BOX_H
#define IC_SELECTION_BOX_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/render_objects/primitivies/line.h>
#include "cross.h"

class SelectionBox : public RenderObject {
private:
    glm::vec2 startPos;
    glm::vec2 endPos;

    std::vector<Line*> lines;
protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

public:

    SelectionBox(SceneID id, std::string name,
                 const glm::vec2& startPos);

    ~SelectionBox();

    std::vector<RenderObject*> scanBox(const Cross* cross, RayCast* rayCast,
                                       int windowWidth, int windowHeight);

    void updateEndPosition(const glm::vec2& endPos);

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;
};


#endif //IC_SELECTION_BOX_H
