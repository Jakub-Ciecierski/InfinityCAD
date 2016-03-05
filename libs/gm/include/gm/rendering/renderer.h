//
// Created by jakub on 3/5/16.
//

#ifndef MG1_RENDERER_H
#define MG1_RENDERER_H


#include <gm/scene/scene.h>

/*
 * Takes ownership over Scene
 */
class Renderer {
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    Scene* scene;

    // Current screen dimensions
    unsigned int width;
    unsigned int height;

    //-----------------------------------------------------------//
    //  PRIVATE METHODS
    //-----------------------------------------------------------//

public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Renderer(Scene* scene);
    Renderer(Scene* scene,
             unsigned int width, unsigned int height);

    ~Renderer();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    Scene* getScene();

    unsigned int getWindowWidth();

    unsigned int getWindowHeight();

    void initialize();

    void render();

    void resize(unsigned int width, unsigned int height);
};


#endif //MG1_RENDERER_H
