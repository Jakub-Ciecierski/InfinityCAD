//
// Created by jakub on 3/6/16.
//

#ifndef MG1_ELLIPSOID_H
#define MG1_ELLIPSOID_H

#include <glm/gtc/matrix_transform.hpp>
#include <gm/color/color.h>

class Ellipsoid {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    float a;
    float b;
    float c;

    glm::mat4 D_MVP;

    Color color;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

    float derivativeX(const glm::vec3& p);
    float derivativeY(const glm::vec3& p);
    float derivativeZ(const glm::vec3& p);

public:

    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Ellipsoid(float a, float b, float c);

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    const Color& getColor();

    void setARadius(float r);
    void setBRadius(float r);
    void setCRadius(float r);

    glm::vec3 derivative(const glm::vec3&p);
    float intersect(float x, float y, const glm::vec3& eye);

    void updateDMVPMatrix(const glm::mat4& MVPMatrix);
};


#endif //MG1_ELLIPSOID_H
