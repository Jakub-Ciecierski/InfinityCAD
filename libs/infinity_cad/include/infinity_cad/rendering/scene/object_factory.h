//
// Created by jakub on 4/3/16.
//

#ifndef MG1_OBJECT_FACTORY_H
#define MG1_OBJECT_FACTORY_H

#include <string>

#include <infinity_cad/rendering/render_objects/primitivies/point.h>
#include <infinity_cad/rendering/render_objects/primitivies/torus.h>
#include <infinity_cad/rendering/render_objects/axis_net.h>
#include <infinity_cad/rendering/render_objects/cross.h>
#include <infinity_cad/rendering/render_objects/cloud.h>
#include <infinity_cad/rendering/render_objects/curves/bspline.h>
#include <infinity_cad/rendering/render_objects/curves/bezier_spline_c0.h>
#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/rendering/projections/projection.h>
#include <infinity_cad/rendering/render_objects/curves/bspline_interp.h>
#include "infinity_cad/rendering/scene/scene_id_factory.h"
#include "scene.h"


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

    Scene* createScene(std::string name);

    Camera* createCameraFPS(std::string name, Projection* projection);

    ifc::Point* createPoint(std::string name);

    Torus* createTorus(std::string name);

    Cross* createCross(std::string name,
                       const std::vector<RenderObject *>* sceneObjects);

    AxisNet* createAxisNet(std::string name,
                           int netSize);

    Cone* createCone(std::string name,
                     float radius, float height, int baseVerticesCount);

    Line* createLine(std::string name,
                     glm::vec4 v1, glm::vec4 v2);

    Cloud* createCloud(std::string name,
                       std::vector<glm::vec4>& points);

    BezierSplineC0 * createBezier(std::string name);
    BezierSplineC0 * createBezier(std::string name,
                                  std::vector<ifc::Point*>& points);

    BezierSplineC2 * createBezierSpline(std::string name,
                                        std::vector<ifc::Point*>& points);
    BezierSplineC2 * createBezierSpline(std::string name);
    
    BSpline* createBSpline(std::string name);

    BSplineInterp* createBSplineInterp(std::string name);

};


#endif //MG1_OBJECT_FACTORY_H
