//
// Created by jakub on 5/2/16.
//

#ifndef IC_BICUBIC_BEZIER_PATCH_H
#define IC_BICUBIC_BEZIER_PATCH_H

#include <glm/gtc/matrix_transform.hpp>
#include <infinity_cad/rendering/render_objects/primitivies/point.h>
#include <math/matrix.h>


class BicubicBezierPatch {
private:
    const int CUBIC_COUNT = 4;

    Matrix<ifc::Point*> points;
    glm::mat4 x;
    glm::mat4 y;
    glm::mat4 z;

public:
    BicubicBezierPatch(Matrix<ifc::Point*> points);
    ~BicubicBezierPatch();

    const glm::mat4& getX() const;
    const glm::mat4& getY() const;
    const glm::mat4& getZ() const;

    const Matrix<ifc::Point*>& getPoints();

    ifc::Point** getColumn(int i);
    /*
     * Memory for the row is allocated
     */
    ifc::Point** getRow(int i);

    void update();
};


#endif //IC_BICUBIC_BEZIER_PATCH_H
