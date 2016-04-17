//
// Created by jakub on 3/29/16.
//

#ifndef MG1_SPHERE_H
#define MG1_SPHERE_H

#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>
#include <infinity_cad/rendering/render_objects/cloud.h>

#include <string>


class Sphere : public RenderObject {
private:
    float radius;
    int ringsCount;
    int sectionCount;

    // s = [0, 2PI]
    // r = [0, PI]
    float getX(float s, float r);
    float getY(float s, float r);
    float getZ(float s, float r);

protected:
    virtual void initVertices() override;
    virtual void initEdges() override;

public:
    Sphere(SceneID,
           float radius, int ringsCount, int sectionCount);
    Sphere(SceneID, std::string name,
           float radius, int ringsCount, int sectionCount);
    ~Sphere();

    Cloud* extractRandomCloud(int verticesCount,
                              float sDivider, float rDivider);
};


#endif //MG1_SPHERE_H