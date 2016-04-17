//
// Created by jakub on 3/6/16.
//

#ifndef MG1_RAY_H
#define MG1_RAY_H

#include <glm/gtc/matrix_transform.hpp>
#include <rc/ellipsoid.h>
#include <infinity_cad/rendering/renderer.h>

class Ray {
private:

    Renderer* renderer;

    void computeTiledWindow(int width, int height,
                            int tileWidthCount, int tileHeightCount);

    void computeTile(int xtileID, int ytileID,
                     int tileWidth, int tileHeight,
                     int restWidth, int restHeight);

    Color computeLightIntensity(const glm::vec3 &p);

    unsigned int tileWidthCount;
    unsigned int tileHeightCount;

public:
    int intesityExponent;

    Ellipsoid* ellipsoid;

    Ray(Renderer* renderer);

    glm::vec3 createRay(float x, float y);

    void rayCasting(Renderer& renderer);

    /*
     * Returns true if reached tileCount = PixelCount
     */
    bool adaptiveRayCasting(Renderer& renderer, int exponent);


    void resetAdaptiveRayCasting();
};


#endif //MG1_RAY_H
