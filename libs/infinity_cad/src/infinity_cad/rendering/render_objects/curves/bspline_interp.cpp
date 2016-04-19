#include <infinity_cad/rendering/render_objects/curves/bspline_interp.h>
#include <infinity_cad/math/math.h>

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BSplineInterp::BSplineInterp(SceneID sceneID, std::string name) :
    Spline(sceneID, name){

}

BSplineInterp::~BSplineInterp(){

}

//-----------------------//
//  PRIVATE
//-----------------------//

void BSplineInterp::computeChordParameters(){
    int n = points.size();
    parameters.clear();
    parameters.resize(n);
    float lengthOfPolygon = 0;
    for(int i = 1; i < n ; i++){
        float dist = ifc::euclideanDistance(points[i]->getPosition(),
                                            points[i-1]->getPosition());
        lengthOfPolygon += dist;
    }
    parameters[0] = 0;
    for(int i = 1; i < n -1; i++){
        float sumOfDistances = 0;
        for(int k = 1; k < i; k++){
            float dist = ifc::euclideanDistance(points[k]->getPosition(),
                                                points[k-1]->getPosition());
            sumOfDistances += dist;
        }
        parameters[i] = sumOfDistances / lengthOfPolygon;
    }
    parameters[n-1] = 1;
}

void BSplineInterp::computeKnotVector(){
    int n = points.size();
    int knotVectorCount = n + DEGREE + 1;

    knotVector.clear();
    knotVector.resize(knotVectorCount);

    for(int i = 0; i <= DEGREE; i++){
        knotVector[i] = 0;
    }
    int j;
    for(j = 1; j < n-DEGREE; j++){
        float sum = 0;
        for(int i = j; i < j + DEGREE; i++){
            sum += parameters[i];
        }
        knotVector[j+ DEGREE] = sum / DEGREE;
    }
    for(int i = j+DEGREE; i < knotVectorCount; i++){
        knotVector[i] = 1;
    }
}

void BSplineInterp::computeControlPoints(){

}


//-----------------------//
//  PROTECTED
//-----------------------//

void BSplineInterp::draw(const glm::mat4 &VP, const Color& color) {

}

void BSplineInterp::buildCurve() {

}


