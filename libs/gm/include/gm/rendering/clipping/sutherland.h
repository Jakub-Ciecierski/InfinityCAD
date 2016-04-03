//
// Created by jakub on 4/2/16.
//

#ifndef MG1_SUTHERLAND_H
#define MG1_SUTHERLAND_H

#include <glm/gtc/matrix_transform.hpp>

static int LEFT=1,RIGHT=2,BOTTOM=4,TOP=8;

int getCode(const glm::vec4& v,
            float left, float right,
            float bottom, float top);

bool clip(glm::vec4& v1, glm::vec4& v2,
          float left, float right,
          float bottom, float top);


#endif //MG1_SUTHERLAND_H
