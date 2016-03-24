//
// Created by jakub on 3/6/16.
//

#include <rc/ray.h>
#include <GL/gl.h>
#include <rc/ray_constants.h>
#include <iostream>

using namespace glm;

Ray::Ray(Renderer* renderer) : renderer(renderer) {
    resetAdaptiveRayCasting();
}

vec3 Ray::createRay(float x, float y){
    Camera* camera = renderer->getScene()->getActiveCamera();
    mat4 projMatrix = camera->getProjection()->getProjectionMatrix();
    mat4 invProjMatrix = inverse(projMatrix);
    mat4 invViewMatrix = inverse(camera->getViewMatrix());

    vec4 clipCoords(x,y, -1, 1);
    vec4 eyeCoords = invProjMatrix * clipCoords;
    eyeCoords.z = -1;
    eyeCoords.w = 0;

    vec4 worldCoords = invViewMatrix * eyeCoords;

    vec3 ray(worldCoords.x, worldCoords.y, worldCoords.z);
    ray = normalize(ray);

    return ray;
}

void Ray::rayCasting(Renderer& renderer) {
    Scene* scene = renderer.getScene();

    unsigned int width = renderer.getWindowWidth();
    unsigned int height = renderer.getWindowHeight();

    ellipsoid->updateDMVPMatrix(scene->getMVP());

    glBegin(GL_POINTS);
    for(unsigned int x = 0; x < width; x++){
        float xGL = renderer.xPixelToGLCoord(x);
        for(unsigned int y = 0; y < height; y++){
            float yGL = renderer.yPixelToGLCoord(y);

            //float z = ellipsoid->intersect(xGL, yGL);
            float z = ellipsoid->intersect(xGL, yGL,
                                           scene->getActiveCamera()->getPosition());

            if(z == RAY_NO_SOLUTION){
                const Color& c = scene->getColor();
                glColor3f(c.R, c.G, c.B);
            }
            else {
                Color surfaceColor = computeLightIntensity(vec3(xGL,yGL, z));
                glColor3f(surfaceColor.R, surfaceColor.G, surfaceColor.B);
            }

            glVertex2f(xGL,
                       yGL);
        }
    }
    glEnd();
}

bool Ray::adaptiveRayCasting(Renderer &renderer, int exponent) {
    bool maxTileCountReached = false;

    Scene* scene = renderer.getScene();
    ellipsoid->updateDMVPMatrix(scene->getMVP());

    unsigned int width = renderer.getWindowWidth();
    unsigned int height = renderer.getWindowHeight();

    computeTiledWindow(width, height,
                       tileWidthCount, tileHeightCount);

    if(tileWidthCount == width && tileHeightCount == height)
        maxTileCountReached = true;

    tileWidthCount *= exponent;
    tileHeightCount *= exponent;

    if(tileWidthCount > width) {
        tileWidthCount = width;
    }
    if(tileHeightCount > height) {
        tileHeightCount = height;
    }

    return maxTileCountReached;
}

void Ray::computeTiledWindow(int width, int height,
                             int tileWidthCount, int tileHeightCount) {
    // add the rest to the last tile
    int restWidth = width % tileWidthCount;
    int restHeight = height % tileHeightCount;

    for(int i = 0; i < tileWidthCount; i++){
        int currentRestWidth = 0;
        int tileWidth = width / tileWidthCount;
        if(i == 0) {
            currentRestWidth = restWidth;
        }

        for(int j = 0; j < tileHeightCount; j++){
            int currentRestHeight = 0;
            int tileHeight = height / tileHeightCount;
            if(j == 0) {
                currentRestHeight = restHeight;
            }
            computeTile(i, j, tileWidth, tileHeight,
                        currentRestWidth, currentRestHeight);
        }
    }
}


void Ray::computeTile(int xtileID, int ytileID,
                      int tileWidth, int tileHeight,
                      int restWidth, int restHeight){
    int xStart = xtileID * tileWidth;
    int yStart = ytileID * tileHeight;

    tileWidth += restWidth;
    tileHeight += restHeight;

    float xStartGL = renderer->xPixelToGLCoord(xStart);
    float yStartGL = renderer->yPixelToGLCoord(yStart);

    // Compute intersectiong of first pixel in tile
    float z = ellipsoid->intersect(xStartGL, yStartGL,
                                   renderer->getScene()->
                                           getActiveCamera()->getPosition());
    glBegin(GL_POINTS);
    if(z == RAY_NO_SOLUTION){
        const Color& c = renderer->getScene()->getColor();
        glColor3f(c.R, c.G, c.B);
    }
    else {
        Color c = computeLightIntensity(vec3(xStartGL, yStartGL, z));
        glColor3f(c.R, c.G, c.B);
    }
    for(int x = xStart; x < xStart + tileWidth; x++){
        for(int y = yStart; y < yStart + tileHeight; y++){
            glVertex2f(renderer->xPixelToGLCoord(x),
                       renderer->yPixelToGLCoord(y));
        }
    }
    glEnd();
}

Color Ray::computeLightIntensity(const vec3& p) {
    const vec3& eyePosition =
            renderer->getScene()->getActiveCamera()->getPosition();
    vec3 N = normalize(ellipsoid->derivative(p));
    vec3 V = normalize(eyePosition - p);

    float dot = (V.x * N.x) + (V.y * N.y) + (V.z * N.z);
    if (dot < 0) dot = 0;
    float lightIntensity = pow(dot, intesityExponent);

    const Color& c = ellipsoid->getColor();

    Color surfaceColor = Color(c.R * lightIntensity,
                               c.G * lightIntensity,
                               c.B * lightIntensity,
                               c.Alpha);

    return surfaceColor;
}

void Ray::resetAdaptiveRayCasting() {
    tileWidthCount = 16;
    tileHeightCount = 16;
}