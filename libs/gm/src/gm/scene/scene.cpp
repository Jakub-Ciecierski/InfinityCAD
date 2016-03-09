//
// Created by jakub on 3/5/16.
//

#include <gm/scene/scene.h>
#include <gm/color/color_settings.h>
#include <GL/gl.h>

Scene::Scene() :
        sceneColor(COLOR_SCENE_DEFAULT), sceneIDFactory(){

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
//  PRIVAte
//--------------------//

void Scene::updateMVP() {
    MVP = activeCamera->getVPMatrix() * getModelMatrix();
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

    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        sceneObjects[i]->render(MVP);
    }
}

void Scene::update() {
    Object::update();

    activeCamera->update();
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        sceneObjects[i]->update();
    }

    updateMVP();
}

