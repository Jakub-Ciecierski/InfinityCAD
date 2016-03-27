//
// Created by jakub on 3/23/16.
//

#ifndef MG1_CROSS_H
#define MG1_CROSS_H

#include <gm/rendering/render_body.h>
#include <map>

class Cross : public RenderBody{
private:
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
    const std::vector<RenderBody*>* sceneObjects;
    std::map<RenderBody*, BodyInfo> grabedMap;

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
    Cross(const std::vector<RenderBody*>* sceneObjects);

    ~Cross();

    void activateGrab();
    void deactivateGrab();

    void scanAndMoveToClosestObject(const RayCast& ray);
    void scanAndMoveToClosestObject(const RayCast& ray, int width, int height);

    virtual float intersect(const RayCast &ray) override;
};


#endif //MG1_CROSS_H
