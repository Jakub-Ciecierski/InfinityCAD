//
// Created by jakub on 4/3/16.
//

#ifndef MG1_OBJECT_FACTORY_H
#define MG1_OBJECT_FACTORY_H

#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/rendering/render_bodies/primitivies/torus.h>

#include <string>
#include <gm/rendering/render_bodies/axis_net.h>
#include <gm/rendering/render_bodies/cross.h>
#include <gm/rendering/render_bodies/cloud.h>
#include <gm/rendering/render_bodies/curves/bezier_curve.h>
#include <gm/rendering/render_bodies/curves/bspline.h>
#include "gm/scene/scene_id_factory.h"

/*
 * Singleton class
 * ObjectFactory is responsible for all object creation.
 * It allocates memory and assings ID for each created object.
 *
 * The memory is NOT deallocated.
 */
class ObjectFactory {
private:
    SceneIDFactory sceneIDFactory;

    ObjectFactory();
public:

    ~ObjectFactory();

    static ObjectFactory& getInstance();

    ic::Point* createPoint(std::string name);

    Torus* createTorus(std::string name);

    Cross* createCross(std::string name,
                       const std::vector<RenderBody*>* sceneObjects);

    AxisNet* createAxisNet(std::string name,
                           int netSize);

    Cone* createCone(std::string name,
                     float radius, float height, int baseVerticesCount);

    Line* createLine(std::string name,
                     glm::vec4 v1, glm::vec4 v2);

    Cloud* createCloud(std::string name,
                       std::vector<glm::vec4>& points);

    BezierCurve* createBezier(std::string name);
    BezierCurve* createBezier(std::string name,
                             std::vector<ic::Point*>& points);

    BSpline* createBSpline(std::string name);

    Camera* createCameraFPS(std::string name,
                            Projection* projection);

};


#endif //MG1_OBJECT_FACTORY_H
