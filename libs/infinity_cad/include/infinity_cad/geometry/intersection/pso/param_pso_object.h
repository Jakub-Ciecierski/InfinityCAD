//
// Created by jakub on 6/13/16.
//

#ifndef IC_PARAM_PSO_OBJECT_H
#define IC_PARAM_PSO_OBJECT_H


#include <pso/pso_object.h>
#include <glm/gtc/matrix_transform.hpp>

class ParamPSOObject : public pso::PSOObject{
public:
    glm::vec2 param1;
    glm::vec2 param2;

    ParamPSOObject();
    ParamPSOObject(glm::vec2 param1, glm::vec2 param2);

    ~ParamPSOObject();

};


#endif //IC_PARAM_PSO_OBJECT_H
