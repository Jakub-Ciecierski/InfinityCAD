//
// Created by jakub on 2/26/16.
//

#ifndef MG1_TORUS_H
#define MG1_TORUS_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <string>

class Torus : public RenderObject {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    float innerRadius;
    float outerRadius;

    unsigned int ringsCount;
    unsigned int sidesCount;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    float getX(float ringAngle, float sideAngle);
    float getY(float ringAngle, float sideAngle);
    float getZ(float ringAngle, float sideAngle);

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

    Torus(SceneID id);
    Torus(SceneID id, std::string name);

    ~Torus();

    void updateNetSize(unsigned int ringsCount, unsigned int sidesCount);
};


#endif //MG1_TORUS_H
