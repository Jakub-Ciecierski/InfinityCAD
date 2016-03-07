//
// Created by jakub on 3/6/16.
//

#include <gm/ray_casting/ray.h>
#include <GL/gl.h>
#include <gm/ray_casting/ray_constants.h>
#include <iostream>

using namespace glm;

Ray::Ray(Renderer* renderer) : renderer(renderer) {

}

void Ray::rayCasting(Renderer& renderer) {
    static int lol = 1;
    Scene* scene = renderer.getScene();
    unsigned int width = renderer.getWindowWidth();
    unsigned int height = renderer.getWindowHeight();

    glBegin(GL_POINTS);
    for(unsigned int x = 0; x < width; x++){
        float xGL = renderer.xPixelToGLCoord(x);
        for(unsigned int y = 0; y < height; y++){
            float yGL = renderer.yPixelToGLCoord(y);

            float z = ellipsoid->intersect(xGL, yGL,
                                           scene->getMVP());

            if(z != RAY_NO_SOLUTION){
                glColor3f(1.0, 0.5, 0);
            }
            else {
                Color c = scene->getColor();

                glColor3f(c.R, c.G, c.B);
            }

            glVertex2f(xGL,
                       yGL);
        }
    }
    glEnd();
}

void Ray::adaptiveRayCasting(Renderer &renderer, int exponent) {
    Scene* scene = renderer.getScene();
    unsigned int width = renderer.getWindowWidth();
    unsigned int height = renderer.getWindowHeight();

    int tileWidthCount = 1;
    int tileHeightCount = 1;
/*
    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
*/
    while(tileWidthCount <= width){
        computeTiledWindow(width, height,
                           tileWidthCount, tileHeightCount);
        tileWidthCount *= exponent;
        tileHeightCount *= exponent;
    }

}

void Ray::computeTiledWindow(int width, int height,
                             int tileWidthCount, int tileHeightCount) {
    // add the rest to the last tile
    int restWidth = width % tileWidthCount;
    int restHeight = height % tileHeightCount;
/*
    std::cout << "---------------------------" << std::endl;
    std::cout << "tileWidthCount: " << tileWidthCount << std::endl;
    std::cout << "tileHeightCount: " << tileHeightCount << std::endl;
*/
    for(int i = 0; i < tileWidthCount; i++){
        int tileWidth = width / tileWidthCount;
        if(i == tileWidthCount-1) tileWidth += restWidth;

        for(int j = 0; j < tileHeightCount; j++){
            int tileHeight = height / tileHeightCount;
            if(j == tileHeightCount-1) tileHeight += restHeight;
            computeTile(i, j, tileWidth, tileHeight);
        }
    }
}

void Ray::computeTile(int xtileID, int ytileID,
                      int tileWidth, int tileHeight) {
    int xStart = xtileID * tileWidth;
    int yStart = ytileID * tileHeight;
    float xStartGL = renderer->xPixelToGLCoord(xStart);
    float yStartGL = renderer->yPixelToGLCoord(yStart);
/*
    std::cout << "Tile (" << xtileID << ", " << ytileID << ")" << std::endl;
    std::cout << "Dim ("
    << tileWidth << ", " << tileHeight  << ")" << std::endl;
*/
    // Compute intersectiong of first pixel in tile
    float z = ellipsoid->intersect(xStartGL, yStartGL,
                                   renderer->getScene()->getMVP());
    Color c = computeLightIntensity(vec4(xStartGL, yStartGL, z, 1));
    glBegin(GL_POINTS);
    glColor3f(c.R, c.G, c.B);
    for(int x = xStart; x < xStart + tileWidth; x++){
        for(int y = yStart; y < yStart + tileHeight; y++){
            glVertex2f(xStartGL, yStartGL);
        }
    }
    glEnd();
}

Color Ray::computeLightIntensity(const vec4 &position) {
    if(position.z != RAY_NO_SOLUTION){
        return Color(1.0, 0.5, 0);
    } else {
        Color c = renderer->getScene()->getColor();
        return Color(c.R, c.G, c.B);
    }
}
