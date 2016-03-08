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

    Color computeLightIntensity(const glm::vec3 &p);

    int tileWidthCount;
    int tileHeightCount;

    struct TileWindow {
        int id;

        int widthWindow;
        int heightWindow;

        int tileWidthCount;
        int tileHeightCount;

        std::vector<std::vector<Color>> bitmap;
    };
public:
    int intesityExponent;

    Ellipsoid* ellipsoid;

    Ray(Renderer* renderer);

    void rayCasting(Renderer& renderer);

    /*
     * Returns true if reached tileCount = PixelCount
     */
    bool adaptiveRayCasting(Renderer& renderer, int exponent);

    bool adaptiveRayCasting_Parallel(Renderer& renderer, int exponent);

    void resetAdaptiveRayCasting();
};


#endif //MG1_RAY_H
