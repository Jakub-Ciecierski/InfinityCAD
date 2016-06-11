//
// Created by jakub on 3/23/16.
//

#ifndef MG1_CROSS_H
#define MG1_CROSS_H


#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/scene/scene_id.h>

#include <infinity_cad/rendering/render_objects/primitivies/cone.h>
#include <infinity_cad/rendering/color/color.h>

#include <map>
#include <string>

enum AxisType{
    XAxis, YAxis, ZAxis, NONE
};

class Cross : public RenderObject {
private:
    bool grabActive;

    glm::vec3 xDirection;
    glm::vec3 yDirection;
    glm::vec3 zDirection;

    Cone* xCone;
    Cone* yCone;
    Cone* zCone;

    Cone* pickedCone;
    std::vector<Cone*> cones;

    Color xAxisColor;
    Color yAxisColor;
    Color zAxisColor;

    Color grabedColor;

    float grabRadius;

    struct BodyInfo{
        BodyInfo(){}
        BodyInfo(float d, Color c) : distance(d), color(c){}
        float distance;
        Color color;
    };

    const std::vector<RenderObject *>* sceneObjects;
    std::map<RenderObject *, BodyInfo> grabedMap;

    void initCones();

protected:
    virtual void translate(float x, float y, float z) override;

    bool isGrabActive;

protected:

    //-----------------------------------------------------------//
    //  PROTECTED METHODS
    //-----------------------------------------------------------//

    virtual void initVertices() override;
    virtual void initEdges() override;

public:
    Cross(SceneID id,
          const std::vector<RenderObject *>* sceneObjects);

    Cross(SceneID id, std::string name,
          const std::vector<RenderObject *>* sceneObjects);


    ~Cross();

    void setGrabActive(bool value);
    bool isGrabAttracting();

    void activateGrab();
    void activateGrab(const std::vector<RenderObject*>& renderObjects);
    void deactivateGrab();
    RenderObject * getClosestObject(const RayCast& ray,
                                    int width, int height,
                                    int distTol = 20) const;
    std::vector<RenderObject*> getClosestObjectVector(const RayCast& ray,
                                                      int width, int height,
                                                      int distTol) const;
    void scanAndMoveToClosestObject(const RayCast& ray, int width, int height);

    void pickCones(const RayCast& ray, int width, int height);
    void unPickCones();
    AxisType getPickedCone();

    const glm::vec3& getXDirection();
    const glm::vec3& getYDirection();
    const glm::vec3& getZDirection();

    virtual void render(const glm::mat4& VP) override;
    virtual void render(const glm::mat4& VP, const Color& color) override;

    virtual void update() override;
};


#endif //MG1_CROSS_H
