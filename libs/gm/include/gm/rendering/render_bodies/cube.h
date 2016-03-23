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

    Cube();

    ~Cube();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    virtual float intersect(const RayCast &ray) override;
};


#endif //MG1_CUBE_H
