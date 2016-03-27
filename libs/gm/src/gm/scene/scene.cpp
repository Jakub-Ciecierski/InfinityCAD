//
// Created by jakub on 3/5/16.
//

#include <gm/scene/scene.h>
#include <gm/color/color_settings.h>
#include <GL/gl.h>
#include <gm/rendering/render_bodies/axis_net.h>
#include <gm/rendering/render_bodies/primitivies/cone.h>

using namespace glm;

Scene::Scene() :
        sceneColor(COLOR_SCENE_DEFAULT), sceneIDFactory(){
    set3DRendering(false);

    cross = new Cross(&this->sceneObjects);
    this->addRenderObject(cross);

    AxisNet* axisNet = new AxisNet(50);
    this->addRenderObject(axisNet);
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

SceneID Scene::addRenderObject(RenderBody *object) {
    SceneID id = sceneIDFactory.createNextAvailableID();

    this->sceneObjects.push_back(object);
    ids.push_back(id);

    return id;
}

void Scene::setActiveRenderBody(const SceneID& id){
    activeRenderBody = getRenderBody(id);
}
RenderBody* Scene::getActiveRenderBody(){
    return activeRenderBody;
}

void Scene::addCamera(Camera *camera) {
    cameras.push_back(camera);
}

bool Scene::setActiveCamera(Camera *camera) {
    activeCamera = camera;

    return true;
}

bool Scene::setActiveCamera(const SceneID &id) {
    return false;
}

bool Scene::removeObject(RigidBody *object) {
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if (object == sceneObjects[i]){
            sceneObjects.erase(sceneObjects.begin()+i);
            return true;
        }
    }
    return false;
}

bool Scene::removeObject(const SceneID &sceneID) {
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if(ids[i].getKey() == sceneID.getKey()){
            ids.erase(ids.begin() + i);
            sceneObjects.erase(sceneObjects.begin() + i);
            return true;
        }
    }
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

const std::vector<RenderBody*>& Scene::getRenderObjects() {
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

RenderBody* Scene::getRenderBody(const SceneID& id){
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if (id.getKey() == ids[i].getKey()){
            return sceneObjects[i];
        }
    }
    return NULL;
}

SceneID Scene::getNextAvailableID(){
    return this->sceneIDFactory.getNextAvailableID();
}

Cross* Scene::getCross(){
    return this->cross;
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
    RigidBody::update();

    activeCamera->update();
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        sceneObjects[i]->update();
    }

    if(!rendering3DEnabled)
        updateMVP();
}
