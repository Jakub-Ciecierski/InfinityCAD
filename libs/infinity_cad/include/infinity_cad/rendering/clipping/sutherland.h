//
// Created by jakub on 4/2/16.
//

#ifndef MG1_SUTHERLAND_H
#define MG1_SUTHERLAND_H

#include <glm/gtc/matrix_transform.hpp>

int getCode(const glm::vec4& v,
            float left, float right,
            float bottom, float top);

bool clip(glm::vec4& v1, glm::vec4& v2,
          float left=-1, float right=1,
          float bottom=-1, float top=1);


#endif //MG1_SUTHERLAND_H
