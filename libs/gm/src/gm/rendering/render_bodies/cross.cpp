//
// Created by jakub on 3/23/16.
//

#include <gm/rendering/render_bodies/cross.h>
#include <gm/color/color_settings.h>
#include <iostream>
#include "gm/rendering/ray_cast.h"
#include "../../../../include/gm/rendering/ray_cast.h"
#include "../../../../include/gm/rendering/render_body.h"

using namespace glm;

Cross::Cross(const std::vector<RenderBody*>* sceneObjects) :
        xAxisColor(COLOR_X_AXIS),
        yAxisColor(COLOR_Y_AXIS),
        zAxisColor(COLOR_Z_AXIS),
        grabedColor(COLOR_GRABED){
    lineWidth = 6.0f;

    grabRadius = 0.1;

    initVertices();
    initEdges();

    this->sceneObjects = sceneObjects;
    isGrabActive = false;
}

Cross::~Cross() {

}

void Cross::translate(float x, float y, float z) {
    RigidBody::translate(x, y, z);

    for(auto &item: grabedMap){
        RenderBody* body = item.first;

        body->move(x, y, z);
    }
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

void Cross::activateGrab() {
    if(isGrabActive) return;
    isGrabActive = true;

    const vec3 crossPos = this->getPosition();
    for(unsigned int i = 1; i < sceneObjects->size(); i++){
        RenderBody* body = (*sceneObjects)[i];
        const vec3& bodyPos = body->getPosition();

        float dx = crossPos.x - bodyPos.x;
        dx *= dx;
        float dy = crossPos.y - bodyPos.y;
        dy *= dy;
        float dz = crossPos.z - bodyPos.z;
        dz *= dz;
        float distance = sqrt(dx + dy + dz);
        if(distance < grabRadius){
            BodyInfo info(distance, *body->getColor());
            grabedMap[body] = info;

            body->setColor(grabedColor);
        }
    }
}

void Cross::deactivateGrab() {
    if(!isGrabActive) return;
    isGrabActive = false;

    for(auto &item: grabedMap){
        RenderBody* body = item.first;
        BodyInfo info = item.second;
        body->setColor(info.color);
    }
    grabedMap.clear();
}

float Cross::intersect(const RayCast &ray) {
    return RAYCAST_NO_SOLUTION;
}

void Cross::scanAndMoveToClosestObject(const RayCast& ray){
    float tol = 0.1;

    RenderBody* closestBody = NULL;
    for(unsigned int i = 1; i < sceneObjects->size(); i++){
        float x = ray.currentX;
        float y = ray.currentY;
        RenderBody* body = (*sceneObjects)[i];


        /*
        float z = body->intersect(ray);
        if(z != RAYCAST_NO_SOLUTION){
            std::cout << "Found object: " << i << std::endl << std::endl;
            this->moveTo(body);
        }else{
            ;//std::cout << "Found nothing" << std::endl;
        }*/
    }
}

void Cross::scanAndMoveToClosestObject(const RayCast& ray, int width, int height){
    int distTol = 20;

    float x = ray.currentX;
    float y = ray.currentY;

    float vX = 2.0 / (float) width;
    int rayX = (x + 1.0f) / vX;

    float vY = 2.0 / (float) height;
    int rayY = (y + 1.0f) / vY;

    RenderBody* closestBody = NULL;
    for(unsigned int i = 1; i < sceneObjects->size(); i++){

        RenderBody* body = (*sceneObjects)[i];

        float bodyX = body->transformed.x;
        float bodyY = body->transformed.y;

        int bodypX = (bodyX + 1.0f) / vX;
        int bodypY  = (bodyY + 1.0f) / vY;

        int dx = rayX - bodypX;
        int dy = rayY - bodypY;
        int dist = sqrt(dx*dx + dy*dy);

        std::cout << "Ray: " << "x:" << rayX << ", y" << rayY << std::endl;
        std::cout << "Body: " << "x:" << bodypX << ", y" << bodypY << std::endl;
        std::cout << "Dist: " << dist << std::endl << std::endl;

        if(dist < distTol){
            this->moveTo(body);
        }

        /*
        float z = body->intersect(ray);
        if(z != RAYCAST_NO_SOLUTION){
            std::cout << "Found object: " << i << std::endl << std::endl;
            this->moveTo(body);
        }else{
            ;//std::cout << "Found nothing" << std::endl;
        }*/
    }
}