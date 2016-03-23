//
// Created by jakub on 3/5/16.
//

#include <gm/rendering/renderer.h>
#include <GL/gl.h>

Renderer::Renderer(Scene *scene) :
        scene(scene){

}

Renderer::Renderer(Scene *scene,
                   unsigned int width, unsigned int height) :
        scene(scene), widthPixels(width), heightPixels(height){

}

Renderer::~Renderer() {
    delete scene;
}

float Renderer::xPixelToGLCoord(int p) {
    float vX = 2.0 / (float) this->getWindowWidth();
    float glX = vX * (float)p - 1.0;

    return glX;
}

float Renderer::yPixelToGLCoord(int p) {
    float vY = 2.0 / (float) this->getWindowHeight();
    float glY = (vY * (float)p - 1.0) * -1;

    return glY;
}

int Renderer::xGLToPixelCoord(float x) {
    float vX = 2.0 / (float) this->getWindowWidth();
    int pX = (x + 1.0f) / vX;

    return pX;
}

int Renderer::yGLToPixelCoord(float y) {
    float vY = 2.0 / (float) this->getWindowHeight();
    int pY = (y + 1.0f) / vY;

    return pY;
}



Scene *Renderer::getScene() {
    return this->scene;
}

unsigned int Renderer::getWindowWidth() {
    return widthPixels;
}

unsigned int Renderer::getWindowHeight() {
    return heightPixels;
}

void Renderer::initialize() {

}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    scene->renderScene();
}

void Renderer::resize(unsigned int width, unsigned int height) {
    this->widthPixels = width;
    this->heightPixels = height;

    glViewport(0, 0, this->widthPixels, this->heightPixels);
}

void Renderer::update(){
    this->scene->update();
}