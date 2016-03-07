//
// Created by jakub on 3/6/16.
//

#ifndef MG1_ELLIPSOID_H
#define MG1_ELLIPSOID_H

#include <glm/gtc/matrix_transform.hpp>

class Ellipsoid {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    float a;
    float b;
    float c;

    glm::mat4 D_MVP;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    void updateDMVPMatrix(const glm::mat4& MVPMatrix);


public:

    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Ellipsoid(float a, float b, float c);

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    float intersect(float x, float y,
                   const glm::mat4& MVPMatrix);

    void setARadius(float r);
    void setBRadius(float r);
    void setCRadius(float r);
};


#endif //MG1_ELLIPSOID_H
