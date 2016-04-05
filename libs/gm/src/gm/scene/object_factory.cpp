//
// Created by jakub on 4/3/16.
//

#include <gm/rendering/render_bodies/primitivies/point.h>
#include <bits/stringfwd.h>
#include <gm/scene/object_factory.h>
#include <gm/cameras/camera_fps.h>

using namespace ic;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

ObjectFactory::ObjectFactory(){

}

ObjectFactory::~ObjectFactory() {

}

//-----------------------//
//  PRIVATE 
//-----------------------//

//-----------------------//
//  PUBLIC
//-----------------------//

ObjectFactory& ObjectFactory::getInstance(){
    static ObjectFactory factory;

    return factory;
}



Point* ObjectFactory::createPoint(std::string name) {
    SceneID id = sceneIDFactory.createNextAvailableID();
    Point* point = new Point(id, name);

    return point;
}

Torus *ObjectFactory::createTorus(std::string name) {
    SceneID id = sceneIDFactory.createNextAvailableID();
    Torus* torus = new Torus(id, name);

    return torus;
}


Cross* ObjectFactory::createCross(std::string name,
                                  const std::vector<RenderBody*>* sceneObjects){
    SceneID id = sceneIDFactory.createNextAvailableID();
    Cross* cross = new Cross(id, name,
                             sceneObjects);

    return cross;
}

AxisNet* ObjectFactory::createAxisNet(std::string name, int netSize){
    SceneID id = sceneIDFactory.createNextAvailableID();

    AxisNet* axisNet = new AxisNet(id, name, netSize);

    return axisNet;
}

Cone* ObjectFactory::createCone(std::string name,
                 float radius, float height, int baseVerticesCount){
    SceneID id = sceneIDFactory.createNextAvailableID();

    Cone* cone = new Cone(id, name,
                          radius, height, baseVerticesCount);

    return cone;
}

Line* ObjectFactory::createLine(std::string name,
                                glm::vec4 v1, glm::vec4 v2){
    SceneID id = sceneIDFactory.createNextAvailableID();

    Line* line = new Line(id, name, v1 ,v2);

    return line;
}

Cloud* ObjectFactory::createCloud(std::string name,
                                  std::vector<glm::vec4>& points){
    SceneID id = sceneIDFactory.createNextAvailableID();

    Cloud* cloud = new Cloud(id, name, points);

    return cloud;
}

BezierCurve* ObjectFactory::createBezier(std::string name){
    SceneID id = sceneIDFactory.createNextAvailableID();

    BezierCurve* bezierCurve = new BezierCurve(id, name);

    return bezierCurve;
}
BezierCurve* ObjectFactory::createBezier(std::string name,
                         std::vector<ic::Point*>& points){
    SceneID id = sceneIDFactory.createNextAvailableID();

    BezierCurve* bezierCurve = new BezierCurve(id, name,
                                               points);
    return bezierCurve;
}

Camera* ObjectFactory::createCameraFPS(std::string name,
                                       Projection* projection){
    SceneID id = sceneIDFactory.createNextAvailableID();

    Camera* camera = new CameraFPS(id, name, projection);

    return camera;
}