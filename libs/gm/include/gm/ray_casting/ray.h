//
// Created by jakub on 3/6/16.
//

#ifndef MG1_RAY_H
#define MG1_RAY_H

#include <glm/gtc/matrix_transform.hpp>
#include <gm/rendering/renderer.h>
#include <gm/ray_casting/ellipsoid.h>

class Ray {
private:

    Renderer* renderer;

    void computeTiledWindow(int width, int height,
                            int tileWidthCount, int tileHeightCount);

    void computeTile(int xtileID, int ytileID,
                     int tileWidth, int tileHeight);

    Color computeLightIntensity(const glm::vec4 &position);

public:
    int intesityExponent;

    Ellipsoid* ellipsoid;

    Ray(Renderer* renderer);

    void rayCasting(Renderer& renderer);

    void adaptiveRayCasting(Renderer& renderer, int exponent);
};


#endif //MG1_RAY_H
