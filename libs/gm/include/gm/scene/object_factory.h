//
// Created by jakub on 4/3/16.
//

#ifndef MG1_OBJECT_FACTORY_H
#define MG1_OBJECT_FACTORY_H

#include <gm/rendering/render_bodies/primitivies/point.h>
#include <gm/rendering/render_bodies/primitivies/torus.h>

#include <string>
#include "scene_id_factory.h"

class ObjectFactory {
private:
    SceneIDFactory sceneIDFactory;

public:

    ObjectFactory();

    ~ObjectFactory();

    Point* createPoint(std::string name);
    Torus* createTorus(std::string name);

};


#endif //MG1_OBJECT_FACTORY_H
