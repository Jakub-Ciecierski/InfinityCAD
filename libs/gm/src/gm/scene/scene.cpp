//
// Created by jakub on 3/5/16.
//

#include <gm/scene/scene.h>
#include <gm/color/color_settings.h>
#include <GL/gl.h>

using namespace glm;

Scene::Scene() :
        sceneColor(COLOR_SCENE_DEFAULT), sceneIDFactory(){
    set3DRendering(false);
}

Scene::~Scene() {
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        delete sceneObjects[i];
    }
    for(unsigned int i = 0; i < cameras.size(); i++){
        delete cameras[i];
    }
}

//--------------------//
//  PRIVATE
//--------------------//

void Scene::updateMVP() {
    MVP = activeCamera->getVPMatrix() * getModelMatrix();
}


void Scene::renderAllObjects() {
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        sceneObjects[i]->render(MVP);
    }
}

void Scene::renderAllObjects3D() {
    const StereoscopicProjection* projection
            = (const StereoscopicProjection*)activeCamera->getProjection();
    mat4 MV = activeCamera->getViewMatrix() * getModelMatrix();

    //mat4 leftMVP = projection->getLeftProjectionMatrix() * MV;
    //mat4 rightMVP = projection->getRightProjectionMatrix() * MV;

    mat4 leftMVP = projection->getLeftProjectionMatrix() *
            activeCamera->getViewMatrix() * getModelMatrix();
    mat4 rightMVP = projection->getRightProjectionMatrix() *
            activeCamera->getViewMatrix() * getModelMatrix();

    Color leftColor = projection->getLeftColor();
    Color rightColor = projection->getRightColor();

    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        glDisable(GL_BLEND);

        sceneObjects[i]->render(leftMVP, leftColor);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE);

        sceneObjects[i]->render(rightMVP, rightColor);

        glDisable(GL_BLEND);
    }
}


//--------------------//
//  SETTERS
//--------------------//

SceneID Scene::addRenderObject(RenderObject *object) {
    this->sceneObjects.push_back(object);

    SceneID id = sceneIDFactory.createNextAvailableID();
    return id;
}

SceneID Scene::addCamera(Camera *camera) {
    cameras.push_back(camera);

    SceneID id = sceneIDFactory.createNextAvailableID();
    return id;
}

bool Scene::setActiveCamera(Camera *camera) {
    activeCamera = camera;

    return true;
}

bool Scene::setActiveCamera(const SceneID &id) {
    return false;
}

bool Scene::removeObject(Object *object) {
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if (object == sceneObjects[i]){
            sceneObjects.erase(sceneObjects.begin()+i);
            return true;
        }
    }
    return false;
}

bool Scene::removeObject(const SceneID &sceneID) {
    return false;
}

void Scene::setColor(Color color) {
    sceneColor = color;

    glClearColor(sceneColor.R, sceneColor.G, sceneColor.B,
                 sceneColor.Alpha);
}


void Scene::set3DRendering(bool v) {
    rendering3DEnabled = v;
}


//--------------------//
//  GETTERS
//--------------------//

const std::vector<RenderObject*>& Scene::getRenderObjects() {
    return this->sceneObjects;
}

const std::vector<Camera*>& Scene::getCameras() {
    return this->cameras;
}


Camera * Scene::getActiveCamera() {
    return this->activeCamera;
}


const Color &Scene::getColor() {
    return this->sceneColor;
}

const glm::mat4& Scene::getMVP() {
    return this->MVP;
}

//--------------------//
//  PUBLIC
//--------------------//

void Scene::renderScene() {
    glClearColor(sceneColor.R, sceneColor.G, sceneColor.B,
                 sceneColor.Alpha);

    if(rendering3DEnabled){
        renderAllObjects3D();
    }else{
        renderAllObjects();
    }
}

void Scene::update() {
    Object::update();

    activeCamera->update();
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        sceneObjects[i]->update();
    }

    if(!rendering3DEnabled)
        updateMVP();
}
