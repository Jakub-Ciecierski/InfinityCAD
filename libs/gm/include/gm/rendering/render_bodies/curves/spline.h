//
// Created by jakub on 4/11/16.
//

#ifndef IC_SPLINE_H
#define IC_SPLINE_H


#include <gm/rendering/render_body.h>
#include <gm/rendering/render_bodies/primitivies/point.h>

/*
 * Pure Abstract class used to create Spline objects
 */
class Spline : public RenderBody {
protected:
    int screenWidth = 1300;
    int screenHeight = 700;

    std::vector<ic::Point*> points;

    Color polygonColor;
    bool doDrawBezierPolygon;

    void initVertices() override;
    virtual void initEdges() override;

    virtual void draw(const glm::mat4 &VP, const Color& color) = 0;

    virtual void buildCurve() = 0;
public:

    Spline(SceneID sceneID, std::string name);

    ~Spline();

    void addPoint(ic::Point* point);
    void removePoint(ic::Point* point);

    void swapPoints(int i, int j);

    void moveUp(ic::Point* point);
    void moveDown(ic::Point* point);

    int getPointIndex(ic::Point* point);
    const std::vector<ic::Point*>& getPoints();

    virtual void setDrawBezierPolygon(bool value);
    bool isDrawBezierPolygon();

    virtual float intersect(const RayCast &ray);
    virtual glm::vec3 getClosestPoint(const glm::vec3 point);

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;

};


#endif //IC_SPLINE_H
