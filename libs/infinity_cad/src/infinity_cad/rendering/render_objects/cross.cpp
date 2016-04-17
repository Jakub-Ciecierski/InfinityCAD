//
// Created by jakub on 3/23/16.
//

#include <infinity_cad/rendering/render_objects/cross.h>
#include <infinity_cad/rendering/color/color_settings.h>
#include <infinity_cad/rendering/scene/object_factory.h>


using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Cross::Cross(SceneID id, const std::vector<RenderObject *>* sceneObjects) :
        RenderObject(id),
        xAxisColor(COLOR_X_AXIS),
        yAxisColor(COLOR_Y_AXIS),
        zAxisColor(COLOR_Z_AXIS),
        grabedColor(COLOR_GRABED){
    lineWidth = 1.5f;
    grabRadius = 0.1;

    initVertices();
    initEdges();
    NDCVertices.resize(vertices.size());

    this->sceneObjects = sceneObjects;
    isGrabActive = false;

    grabable = false;
    initCones();
}

Cross::Cross(SceneID id, std::string name,
             const std::vector<RenderObject *>* sceneObjects) :
        RenderObject(id, name),
        xAxisColor(COLOR_X_AXIS),
        yAxisColor(COLOR_Y_AXIS),
        zAxisColor(COLOR_Z_AXIS),
        grabedColor(COLOR_GRABED){
    lineWidth = 1.5f;
    grabRadius = 0.1;

    initVertices();
    initEdges();
    NDCVertices.resize(vertices.size());

    this->sceneObjects = sceneObjects;
    isGrabActive = false;

    grabable = false;
    initCones();
}

Cross::~Cross() {
    delete xCone;
    delete yCone;
    delete zCone;
}

//-----------------------//
//  PRIVATE
//-----------------------//


void Cross::initCones(){
    float r = 0.2;
    float h = 0.6;
    int baseCount = 30;

    float scaleFactor = 0.1;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    xCone = objectFactory.createCone("X Cone", r, h, baseCount);
    yCone = objectFactory.createCone("Y Cone", r, h, baseCount);
    zCone = objectFactory.createCone("Z Cone", r, h, baseCount);

    xCone->setColor(xAxisColor);
    xCone->scale(scaleFactor );
    xCone->rotate(0, 0, 180 + 90);
    xCone->move(grabRadius - ((h-0.1)*scaleFactor), 0, 0);

    yCone->setColor(yAxisColor);
    yCone->scale(scaleFactor );
    yCone->rotate(0, 180, 0);
    yCone->move(0, grabRadius - ((h-0.1)*scaleFactor), 0);

    zCone->setColor(zAxisColor);
    zCone->scale(scaleFactor );
    zCone->rotate(90, 0, 0);
    zCone->move(0, 0, grabRadius - ((h-0.1)*scaleFactor));

}

//-----------------------//
//  PROTECTED
//-----------------------//

void Cross::translate(float x, float y, float z) {
    RigidObject::translate(x, y, z);

    for(auto &item: grabedMap){
        RenderObject * body = item.first;

        body->move(x, y, z);
    }

    xCone->move(x, y, z);
    yCone->move(x, y, z);
    zCone->move(x, y, z);
}

void Cross::initVertices() {
    vertices.resize(6);

    // x axis
    vertices[0].x = -1;
    vertices[0].y = 0;
    vertices[0].z = 0;
    vertices[0].w = 1;

    vertices[1].x = 1;
    vertices[1].y = 0;
    vertices[1].z = 0;
    vertices[1].w = 1;

    // y axis
    vertices[2].x = 0;
    vertices[2].y = 1;
    vertices[2].z = 0;
    vertices[2].w = 1;

    vertices[3].x = 0;
    vertices[3].y = -1;
    vertices[3].z = 0;
    vertices[3].w = 1;


    // z axis
    vertices[4].x = 0;
    vertices[4].y = 0;
    vertices[4].z = 1;
    vertices[4].w = 1;

    vertices[5].x = 0;
    vertices[5].y = 0;
    vertices[5].z = -1;
    vertices[5].w = 1;

    float scaleFactor = grabRadius;
    vec3 scaleVec(scaleFactor,scaleFactor,scaleFactor);
    for(int i = 0; i < 6; i++){
        vertices[i] =
                glm::scale(glm::mat4(1.0f), scaleVec) * vertices[i];
    }
}

void Cross::initEdges() {
    Edge e1(0,1);
    e1.color = &xAxisColor;

    Edge e2(2,3);
    e2.color = &yAxisColor;

    Edge e3(4,5);
    e3.color = &zAxisColor;

    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
}


//-----------------------//
//  PUBLIC
//-----------------------//


void Cross::activateGrab() {
    if(isGrabActive) return;
    isGrabActive = true;

    float minDistance = 1000;
    RenderObject * minBody = NULL;

    // TODO children hack
    const vec3 crossPos = this->getPosition();
    std::vector<RenderObject *> allSceneObjects;
    for(unsigned int i = 0; i < sceneObjects->size(); i++){
        RenderObject * body = (*sceneObjects)[i];
        allSceneObjects.push_back(body);
        const std::vector<RenderObject *>& children = body->getChildren();
        for(unsigned int j = 0; j < children.size(); j++){
            allSceneObjects.push_back(children[j]);
        }
    }

    for(unsigned int i = 0; i < allSceneObjects.size(); i++){
        RenderObject * body = (allSceneObjects)[i];
        if(!body->isGrabable()) continue;

        const vec3& bodyPos = body->getPosition();

        float dx = crossPos.x - bodyPos.x;
        dx *= dx;
        float dy = crossPos.y - bodyPos.y;
        dy *= dy;
        float dz = crossPos.z - bodyPos.z;
        dz *= dz;
        float distance = sqrt(dx + dy + dz);
        if(distance < grabRadius){
            if(distance < minDistance){
                minDistance = distance;
                minBody = body;
            }
        }
    }
    if(minBody != NULL){
        grabedMap.clear();
        grabedMap[minBody] = BodyInfo(minDistance, *minBody->getColor());
        minBody->setColor(grabedColor);
        minBody->setSelected(true);
    }


}

void Cross::deactivateGrab() {
    if(!isGrabActive) return;
    isGrabActive = false;

    for(auto &item: grabedMap){
        RenderObject * body = item.first;
        BodyInfo info = item.second;
        body->setColor(info.color);
        body->setSelected(false);
    }
    grabedMap.clear();
}

RenderObject * Cross::getClosestObject(const RayCast& ray,
                                       int width, int height){
    int distTol = 20;

    float x = ray.currentX;
    float y = ray.currentY;

    float vX = 2.0 / (float) width;
    int rayX = (x + 1.0f) / vX;

    float vY = 2.0 / (float) height;
    int rayY = (y + 1.0f) / vY;

    for(unsigned int i = 1; i < sceneObjects->size(); i++){


        RenderObject * body = (*sceneObjects)[i];

        // ---------------------
        // TODO children
        const std::vector<RenderObject *>& curr_children = body->getChildren();
        for(unsigned int i = 0 ; i < curr_children.size(); i++){
            RenderObject * child = curr_children[i];

            const vec3& bodyProjectedPosition = child->getProjectedPosition();

            float bodyX = bodyProjectedPosition.x;
            float bodyY = bodyProjectedPosition.y;

            int bodypX = (bodyX + 1.0f) / vX;
            int bodypY  = (bodyY + 1.0f) / vY;

            int dx = rayX - bodypX;
            int dy = rayY - bodypY;
            int dist = sqrt(dx*dx + dy*dy);

            if(dist < distTol){
                return (child);
            }
        }
        // ---------------------

        if(!body->isGrabable()) continue;

        const vec3& bodyProjectedPosition = body->getProjectedPosition();

        float bodyX = bodyProjectedPosition.x;
        float bodyY = bodyProjectedPosition.y;

        int bodypX = (bodyX + 1.0f) / vX;
        int bodypY  = (bodyY + 1.0f) / vY;

        int dx = rayX - bodypX;
        int dy = rayY - bodypY;
        int dist = sqrt(dx*dx + dy*dy);

        if(dist < distTol){
            return (body);
        }

        /*
        RenderBody* body = (*sceneObjects)[i];

        if(!body->isGrabable()) continue;
        const vec3& bodyProjectedPosition = body->getProjectedPosition();

        float bodyX = bodyProjectedPosition.x;
        float bodyY = bodyProjectedPosition.y;

        int bodypX = (bodyX + 1.0f) / vX;
        int bodypY  = (bodyY + 1.0f) / vY;

        int dx = rayX - bodypX;
        int dy = rayY - bodypY;
        int dist = sqrt(dx*dx + dy*dy);

        if(dist < distTol){
            return (body);
        }
         */

    }
    return NULL;
}

void Cross::scanAndMoveToClosestObject(const RayCast& ray, int width, int height){
    int distTol = 20;

    float x = ray.currentX;
    float y = ray.currentY;

    float vX = 2.0 / (float) width;
    int rayX = (x + 1.0f) / vX;

    float vY = 2.0 / (float) height;
    int rayY = (y + 1.0f) / vY;

    for(unsigned int i = 1; i < sceneObjects->size(); i++){

        RenderObject * body = (*sceneObjects)[i];
        const vec3& bodyProjectedPosition = body->getProjectedPosition();

        float bodyX = bodyProjectedPosition.x;
        float bodyY = bodyProjectedPosition.y;

        int bodypX = (bodyX + 1.0f) / vX;
        int bodypY  = (bodyY + 1.0f) / vY;

        int dx = rayX - bodypX;
        int dy = rayY - bodypY;
        int dist = sqrt(dx*dx + dy*dy);

        if(dist < distTol){
            this->moveTo(body);
        }

    }
}

void Cross::render(const glm::mat4 &VP) {
    RenderObject::render(VP);

    xCone->render(VP);
    yCone->render(VP);
    zCone->render(VP);

}

void Cross::render(const glm::mat4 &VP, const Color &color) {
    RenderObject::render(VP, color);

    xCone->render(VP, color);
    yCone->render(VP, color);
    zCone->render(VP, color);
}

void Cross::update() {
    RigidObject::update();

    xCone->update();
    yCone->update();
    zCone->update();
}

