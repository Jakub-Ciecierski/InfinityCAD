//
// Created by jakub on 2/28/16.
//

#ifndef MG1_CUBE_H
#define MG1_CUBE_H

#include <vector>
#include <gm/rendering/render_body.h>

class Cube : public RenderBody {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//
    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

protected:

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    virtual void initVertices() override;
    virtual void initEdges() override;

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Cube(SceneID id);
    Cube(SceneID id, std::string name);

    ~Cube();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual float intersect(const RayCast &ray) override;

    virtual glm::vec3 getClosestPoint(const glm::vec3 point) override;
};


#endif //MG1_CUBE_H
