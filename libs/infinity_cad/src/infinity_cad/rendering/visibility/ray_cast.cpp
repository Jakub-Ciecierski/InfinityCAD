//
// Created by jakub on 3/23/16.
//

#include <infinity_cad/rendering/visibility/ray_cast.h>
#include <infinity_cad/rendering/cameras/camera.h>

#include <GL/gl.h>

using namespace glm;

RayCast::RayCast(Camera *camera){
    this->camera = camera;
}

RayCast::~RayCast() {

}

void RayCast::update(float x, float y) {
    currentX = x;
    currentY = y;
    /*
    mat4 projMatrix = camera->getProjection()->getProjectionMatrix();
    mat4 invProjMatrix = inverse(projMatrix);
    mat4 invViewMatrix = inverse(camera->getViewMatrix());

    vec4 clipCoords(x, y, 1, 1);
    vec4 eyeCoords = invProjMatrix * clipCoords;
    eyeCoords.z = 1;
    eyeCoords.w = 0;

    vec4 worldCoords = invViewMatrix * eyeCoords;
    vec3 ray(worldCoords.x, worldCoords.y, worldCoords.z);

    direction = normalize(ray);
    origin = camera->getPosition();
*/


}
void RayCast::update(float x, float y,
                     int width, int height) {
    currentX = x;
    currentY = y;
    /*
    mat4 projMatrix = camera->getProjection()->getProjectionMatrix();
    mat4 invProjMatrix = inverse(projMatrix);
    mat4 invViewMatrix = inverse(camera->getViewMatrix());

    vec4 clipCoords(x, y, 1, 1);
    vec4 eyeCoords = invProjMatrix * clipCoords;
    eyeCoords.z = 1;
    eyeCoords.w = 0;

    vec4 worldCoords = invViewMatrix * eyeCoords;
    vec3 ray(worldCoords.x, worldCoords.y, worldCoords.z);

    direction = normalize(ray);
    origin = camera->getPosition();
*/
    direction = camera->getDirection();
    origin = camera->getPosition();

    const float aspect = float(height) / width;

    const float s = -2.0f * tan(45 * 0.5f);

    const vec3& start = vec3( ((x + 0.5)/ width ) * s,
                              -((y+0.5) / height) * s * aspect, 1.0f) * 0.1f;

    origin = start;
    direction = start - camera->getPosition();
    direction = normalize(direction);


}

const vec3& RayCast::getOrigin() const{
    return this->origin;
}

const vec3& RayCast::getDirection() const{
    return this->direction;
}

void RayCast::render(const mat4& MVP) const {
    float t = 1;
    vec4 vec1 = MVP * vec4(origin.x, origin.y, origin.z, 1.0f);
    vec4 vec2 = MVP * vec4(origin.x + (direction.x * t),
                           origin.y + (direction.y * t),
                           origin.z + (direction.z * t),
                           1.0f);
    vec1.x /= vec1.w;
    vec1.y /= vec1.w;

    vec2.x /= vec2.w;
    vec2.y /= vec2.w;

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glVertex2f(vec1.x,
               vec1.y);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(vec2.x,
               vec2.y);

    glEnd();
}
