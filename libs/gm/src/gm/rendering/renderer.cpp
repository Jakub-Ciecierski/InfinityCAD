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
    scene(scene), width(width), height(height){

}

Renderer::~Renderer() {
    delete scene;
}

Scene *Renderer::getScene() {
    return this->scene;
}

unsigned int Renderer::getWindowWidth() {
    return width;
}

unsigned int Renderer::getWindowHeight() {
    return height;
}

void Renderer::initialize() {

}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    scene->renderScene();
}

void Renderer::resize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;

    glViewport(0, 0, this->width, this->height);
}
