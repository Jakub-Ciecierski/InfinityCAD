//
// Created by jakub on 4/11/16.
//

#ifndef IC_SPLINE_H
#define IC_SPLINE_H


#include <infinity_cad/rendering/color/color.h>
#include <infinity_cad/rendering/scene/scene_id.h>
#include <infinity_cad/rendering/render_objects/primitivies/point.h>
#include <infinity_cad/rendering/visibility/ray_cast.h>
#include <infinity_cad/rendering/render_object.h>

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

/*
 * Pure Abstract class used to create Spline objects
 */
class Spline : public RenderObject {
protected:
    int screenWidth = 1300;
    int screenHeight = 700;

    Color polygonColor;
    bool doDrawBezierPolygon;

    std::vector<ifc::Point*> points;

    void initVertices() override;
    virtual void initEdges() override;

    virtual void draw(const glm::mat4 &VP, const Color& color) = 0;
    virtual void buildCurve() = 0;
public:

    Spline(SceneID sceneID, std::string name);

    ~Spline();

    void addPoint(ifc::Point* point);
    void removePoint(ifc::Point* point);

    void swapPoints(int i, int j);

    void moveUp(ifc::Point* point);
    void moveDown(ifc::Point* point);

    int getPointIndex(ifc::Point* point);

    virtual void setDrawBezierPolygon(bool value);
    bool isDrawBezierPolygon();

    const std::vector<ifc::Point*>& getControlPoints();

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;

};


#endif //IC_SPLINE_H
