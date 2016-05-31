//
// Created by jakub on 3/5/16.
//

#include <infinity_cad/rendering/scene/scene.h>
#include <infinity_cad/rendering/color/color_settings.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/rendering/projections/stereoscopic_projection.h>


using namespace glm;

Scene::Scene(SceneID ID, std::string name) :
        RigidObject(ID, name),
        sceneColor(COLOR_SCENE_DEFAULT), sceneIDFactory(){
    set3DRendering(false);


    initSceneElements();

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

void Scene::initSceneElements(){
    ObjectFactory objectFactory = ObjectFactory::getInstance();
    
    cross = objectFactory.createCross("Cross Grabber", &this->sceneObjects);
    this->addRenderObject(cross);

    AxisNet* axisNet = objectFactory.createAxisNet("Axis Net", 20);
    this->addRenderObject(axisNet);
}

void Scene::updateMVP() {
    MVP = activeCamera->getVPMatrix() * this->getModelMatrix();
}


void Scene::renderAllObjects() {
    glClearColor(sceneColor.R, sceneColor.G, sceneColor.B,
                 sceneColor.Alpha);

    int size = sceneObjects.size();
    for(int i = 0; i < size; i++){
        sceneObjects[i]->render(MVP);
    }
    cross->render(MVP);
}

void Scene::renderAllObjects3D() {
    const StereoscopicProjection* projection
            = (const StereoscopicProjection*)activeCamera->getProjection();
    mat4 MV = activeCamera->getViewMatrix() * this->getModelMatrix();

    mat4 leftMVP = projection->getLeftProjectionMatrix() * MV;
    mat4 rightMVP = projection->getRightProjectionMatrix() * MV;

    Color leftColor = projection->getLeftColor();
    Color rightColor = projection->getRightColor();

    glClearColor(COLOR_BLACK.R, COLOR_BLACK.G, COLOR_BLACK.B,
                 COLOR_BLACK.Alpha);

    int size = sceneObjects.size();
    for(int i = 0; i < size; i++){
        glDisable(GL_BLEND);

        sceneObjects[i]->render(leftMVP, leftColor);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE);

        sceneObjects[i]->render(rightMVP, rightColor);

        glDisable(GL_BLEND);
    }

    glDisable(GL_BLEND);

    cross->render(leftMVP, leftColor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE);

    cross->render(rightMVP, rightColor);

    glDisable(GL_BLEND);
}


//--------------------//
//  SETTERS
//--------------------//

void Scene::addRenderObjectFront(RenderObject *object){
    this->sceneObjects.insert(sceneObjects.begin(), object);
}

void Scene::addRenderObject(RenderObject *object) {
    this->sceneObjects.push_back(object);
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

bool Scene::removeObject(RigidObject *object) {
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if (object == sceneObjects[i]){
            sceneObjects.erase(sceneObjects.begin()+i);
            delete object;
            return true;
        }
    }
    return false;
}

bool Scene::removeObject(const SceneID &sceneID) {
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if (sceneID == sceneObjects[i]->getID()){
            // TODO check if works
            RenderObject * renderBody = sceneObjects[i];
            sceneObjects.erase(sceneObjects.begin()+i);
            delete renderBody;
            return true;
        }
    }
    return false;
}

void Scene::setColor(Color color) {
    sceneColor = color;

    // TODO out of place code
    glClearColor(sceneColor.R, sceneColor.G, sceneColor.B,
                 sceneColor.Alpha);
}


void Scene::set3DRendering(bool v) {
    rendering3DEnabled = v;
}


//--------------------//
//  GETTERS
//--------------------//

const std::vector<RenderObject *>& Scene::getRenderObjects() {
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

RenderObject * Scene::getRenderBody(const SceneID& id){
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if (id == sceneObjects[i]->getID()){
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

std::vector<RenderObject*> Scene::getObjects(const ObjectType& type){
    std::vector<RenderObject*> renderObjects;
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        if(type == sceneObjects[i]->getType()){
            renderObjects.push_back(sceneObjects[i]);
        }
    }

    return renderObjects;
}

//--------------------//
//  PUBLIC
//--------------------//

void Scene::renderScene() {
    glClearColor(sceneColor.R, sceneColor.G, sceneColor.B,
                 sceneColor.Alpha);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    if(rendering3DEnabled){
        renderAllObjects3D();
    }else{
        renderAllObjects();
    }

    glDisable(GL_BLEND);
}

void Scene::update() {
    RigidObject::update();

    activeCamera->update();
    updateMVP();

    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        sceneObjects[i]->update();
    }
}

void Scene::UPDATE_TEST(){
    for(unsigned int i = 0; i < sceneObjects.size(); i++){
        sceneObjects[i]->update();
        std::cout << &sceneObjects[i] << std::endl;
    }
}