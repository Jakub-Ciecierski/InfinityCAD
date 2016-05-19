#include <infinity_cad/rendering/render_objects/curves/bspline_interp.h>
#include <infinity_cad/math/math.h>
#include <infinity_cad/geometry/polynomials/bspline_basis.h>
#include <infinity_cad/settings/settings.h>
#include <ifc_gpu/splines/bspline_gpu.h>
#include <iostream>
#include <math/band_matrix/band_matrix_solve.h>

using namespace std;
using namespace glm;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

BSplineInterp::BSplineInterp(SceneID sceneID, std::string name) :
    Spline(sceneID, name){
    curvePoints = (vec4*)malloc(sizeof(vec4) * MAX_PIXEL_COUNT);

    objectType = OBJ_TYPE_CURVE_INTERP;
}

BSplineInterp::~BSplineInterp(){
    delete curvePoints;
}

//-----------------------//
//  PRIVATE
//-----------------------//

void BSplineInterp::computeChordParameters(){
    int n = points.size();
    parameters.clear();
    parameters.resize(n);

    interpolatingPolygonLength = 0;
    for(int i = 1; i < n ; i++){
        float dist = ifc::euclideanDistance(points[i]->getPosition(),
                                            points[i-1]->getPosition());
        interpolatingPolygonLength += dist;
    }
    parameters[0] = 0;
    for(int i = 1; i < n -1; i++){
        float sumOfDistances = 0;
        for(int k = 1; k <= i; k++){
            float dist = ifc::euclideanDistance(points[k]->getPosition(),
                                                points[k-1]->getPosition());
            sumOfDistances += dist;
        }
        parameters[i] = sumOfDistances / interpolatingPolygonLength;
    }
    parameters[n-1] = 1.0f;
}

void BSplineInterp::computeKnotVector(int DEGREE){
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
    float l = 1.0f;
    for(int i = j+DEGREE; i < knotVectorCount; i++){
        knotVector[i] = l;
    }
}

void BSplineInterp::computeControlPoints(int degree){
    if(degree == 3){
        computeControlPointsDeg3(degree);
    }
    if(degree == 2 || degree == 1){
        computeControlPointsDeg12(degree);
    }
}

void BSplineInterp::computeControlPointsDeg3(int DEGREE){
    int n = points.size();

    const int SUB_DIAG_COUNT = 2;
    const int SUPER_DIAG_COUNT = 2;
    const int DIAG_COUNT = 5;

    float** bandMatrix = (float**)malloc(sizeof(float*) * (n+1));
    float** lowerMatrix = (float**)malloc(sizeof(float*) * (n+1));
    for(int i = 1; i < n+1;i++){
        bandMatrix[i] = (float*)malloc(sizeof(float) * (DIAG_COUNT+1));
        lowerMatrix[i] = (float*)malloc(sizeof(float) * (SUB_DIAG_COUNT+1));
    }

    unsigned long* index = (unsigned long*)malloc(sizeof(unsigned long) *
                                                  (n+1));

    // ---------------------------------------
    bandMatrix[1][1] = 0;
    bandMatrix[2][1] = 0;
    for(int i = 3; i < n+1; i++){
        bandMatrix[i][1] = bsplineRecurive(parameters[i-1], DEGREE, i-3,
                                           knotVector);
    }

    bandMatrix[1][2] = 0;
    for(int i = 2; i < n+1; i++){
        bandMatrix[i][2] = bsplineRecurive(parameters[i-1], DEGREE, i - 2,
                                           knotVector);
    }

    for(int i = 1; i < n+1; i++){
        bandMatrix[i][3] = bsplineRecurive(parameters[i-1], DEGREE, i-1,
                                           knotVector);
    }

    for(int i = 1; i < n; i++){
        bandMatrix[i][4] = bsplineRecurive(parameters[i-1], DEGREE, i,
                                           knotVector);
    }
    bandMatrix[n][4] = 0;

    for(int i = 1; i < n-1; i++){
        bandMatrix[i][5] = bsplineRecurive(parameters[i-1], DEGREE, i+1,
                                           knotVector);
    }
    bandMatrix[n][5] = 0;
    bandMatrix[n-1][5] = 0;
    // ---------------------------------------

    float evenOdd = 0;
    bandec(bandMatrix, n, SUB_DIAG_COUNT, SUPER_DIAG_COUNT, lowerMatrix,
           index, &evenOdd);

    controlPoints.clear();
    controlPoints.resize(n);
    for(int s = 0; s < DIMENSION; s++){
        float* d = (float*)malloc(sizeof(float) * (n+1));

        for(int i = 1; i < n+1; i++){
            const vec3& pos = points[i-1]->getPosition();
            d[i] = pos[s];
        }

        banbks(bandMatrix, n , SUB_DIAG_COUNT, SUPER_DIAG_COUNT, lowerMatrix,
               index, d);

        for(int i = 1; i < n+1; i++){
            vec3& v = controlPoints[i-1];
            v[s] = d[i];
        }
        delete d;
    }
    for(int i = 1; i < n+1; i++){
        delete bandMatrix[i];
        delete lowerMatrix[i];
    }
    delete index;
    delete bandMatrix;
    delete lowerMatrix;
}

void BSplineInterp::computeControlPointsDeg12(int DEGREE){
    int n = points.size();

    const int SUB_DIAG_COUNT = 1;
    const int SUPER_DIAG_COUNT = 1;
    const int DIAG_COUNT = 3;

    float** bandMatrix = (float**)malloc(sizeof(float*) * (n+1));
    float** lowerMatrix = (float**)malloc(sizeof(float*) * (n+1));
    for(int i = 1; i < n+1;i++){
        bandMatrix[i] = (float*)malloc(sizeof(float) * (DIAG_COUNT+1));
        lowerMatrix[i] = (float*)malloc(sizeof(float) * (SUB_DIAG_COUNT+1));
    }

    unsigned long* index = (unsigned long*)malloc(sizeof(unsigned long) *
                                                  (n+1));

    bandMatrix[1][1] = 0;
    for(int i = 2; i < n+1; i++){
        bandMatrix[i][1] = bsplineRecurive(parameters[i-1], DEGREE, i - 2,
                                           knotVector);
    }

    for(int i = 1; i < n+1; i++){
        bandMatrix[i][2] = bsplineRecurive(parameters[i-1], DEGREE, i-1,
                                           knotVector);
    }

    for(int i = 1; i < n; i++){
        bandMatrix[i][3] = bsplineRecurive(parameters[i-1], DEGREE, i,
                                           knotVector);
    }
    bandMatrix[n][3] = 0;

    // ---------------------------------------

    float evenOdd = 0;
    bandec(bandMatrix, n, SUB_DIAG_COUNT, SUPER_DIAG_COUNT, lowerMatrix,
           index, &evenOdd);

    controlPoints.clear();
    controlPoints.resize(n);
    for(int s = 0; s < DIMENSION; s++){
        float* d = (float*)malloc(sizeof(float) * (n+1));

        for(int i = 1; i < n+1; i++){
            const vec3& pos = points[i-1]->getPosition();
            d[i] = pos[s];
        }

        banbks(bandMatrix, n , SUB_DIAG_COUNT, SUPER_DIAG_COUNT, lowerMatrix,
               index, d);

        for(int i = 1; i < n+1; i++){
            vec3& v = controlPoints[i-1];
            v[s] = d[i];
        }
        delete d;
    }
    for(int i = 1; i < n+1; i++){
        delete bandMatrix[i];
        delete lowerMatrix[i];
    }
    delete index;
    delete bandMatrix;
    delete lowerMatrix;
}

void BSplineInterp::drawSplineCPU(const glm::mat4 &VP, const Color& color,
                            int degree, float t, float t_max, float dt){
    setSurfaceColor(color);
    glPointSize(1.0f);
    glBegin(GL_POINTS);

    while(t < t_max){
        vec4 point = computeBSpline(controlPoints,
                                    knotVector, t, degree);
        t += dt;

        point = VP * point;
        point.x /= point.w;
        point.y /= point.w;

        if(point.w < 0) continue; // clip
        glVertex2f(point.x, point.y);
    }

    glEnd();
}
void BSplineInterp::drawSplineGPU(const glm::mat4 &VP, const Color& color,
                            int pixelCount, float t, float dt, int degree){
    int pointSize = controlPoints.size();

    if(pixelCount > MAX_PIXEL_COUNT){
        delete curvePoints;
        MAX_PIXEL_COUNT = pixelCount;
        curvePoints = (vec4*) malloc(MAX_PIXEL_COUNT * sizeof(vec4));
    }

    ifc_gpu::computeBSpline(controlPoints.data(), pointSize,
                            knotVector.data(), knotVector.size(),
                            curvePoints, pixelCount, t, dt, degree, &VP);

    setSurfaceColor(color);
    glPointSize(1.0f);
    glBegin(GL_POINTS);

    for(int i = 0; i < pixelCount; i++){
        if(curvePoints[i].w < 0) continue; // clip
        glVertex2f(curvePoints[i].x, curvePoints[i].y);
    }

    glEnd();
}

//-----------------------//
//  PROTECTED
//-----------------------//

void BSplineInterp::draw(const glm::mat4 &VP, const Color& color) {
    updateCurve();

    int n = points.size();
    if(n < 2) return;
    int degree = DEGREE;
    if(n < MIN_POINTS){
        degree = DEGREE - (MIN_POINTS - n);
    }
    if(degree == 0) return;

    float t_max = 1.0f;
    float t = 0.0f;

    float sceenLength = 4*(screenWidth + screenHeight);
    float dt = 1/(sceenLength * interpolatingPolygonLength);

    int pixelCount = (t_max - t) / dt;

    if(ifc::RUN_CUDA){
        drawSplineGPU(VP, color, pixelCount, t, dt, degree);
    }else{
        drawSplineCPU(VP, color, degree, t, t_max, dt);
    }

}

void BSplineInterp::updateCurve() {
    int n = points.size();
    if(n < 2) return;
    int degree = DEGREE;
    if(n < MIN_POINTS){
        degree = DEGREE - (MIN_POINTS - n);
    }

    computeChordParameters();
    computeKnotVector(degree);
    computeControlPoints(degree);
}

void BSplineInterp::buildCurve(){

}
