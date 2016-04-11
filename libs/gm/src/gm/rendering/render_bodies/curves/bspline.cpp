//
// Created by jakub on 4/10/16.
//

#include <gm/rendering/render_bodies/curves/bspline.h>

using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BSpline::BSpline(SceneID id, std::string name) :
    Spline(id, name){

}

BSpline::~BSpline(){

}

//-----------------------//
//  PROTECTED
//-----------------------//

void BSpline::initVertices(){



}

void BSpline::initEdges() {

}


void BSpline::draw(const glm::mat4 &VP, const Color& color) {

}

void BSpline::buildCurve() {
    
}

//-----------------------//
//  PUBLIC 
//-----------------------//



float BSpline::intersect(const RayCast &ray) {
    return RAYCAST_NO_SOLUTION;
}

glm::vec3 BSpline::getClosestPoint(const glm::vec3 point) {
    return vec3();
}
