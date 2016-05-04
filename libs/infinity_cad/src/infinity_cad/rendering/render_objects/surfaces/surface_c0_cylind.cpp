//
// Created by jakub on 5/4/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface_c0_cylind.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/settings/settings.h>
#include <infinity_cad/geometry/polynomials/bernstein_basis.h>
#include <infinity_cad/math/math.h>
#include <algorithm>
#include <ifc_gpu/surfaces/bezier_surface_gpu.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

SurfaceC0Cylind::SurfaceC0Cylind(SceneID id, std::string name,
                             int n, int m, float radius, float height) :
        Surface(id, name, n, m), radius(radius), height(height),
        patches(n,m, NULL){
    this->build();

    surfacePixels = (vec4 *) malloc(MAX_PIXEL_COUNT * sizeof(vec4));
}

SurfaceC0Cylind::~SurfaceC0Cylind(){
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            delete patches[i][j];
        }
    }
    delete surfacePixels;
}

//-----------------------//
//  PRIVATE
//-----------------------//

Matrix<ifc::Point*> SurfaceC0Cylind::getMatrixFormOfAllPatches(){
    int numberOfPointsInRow = CUBIC_COUNT*n - (n-1) - 1;
    int numberOfPointsInColumns = CUBIC_COUNT*m - (m-1);

    vector<vector<ifc::Point*>> points(numberOfPointsInColumns);

    for(int i = 0; i < n; i++){
        int currentRow = 0;
        for(int j = 0; j < m; j++){
            BicubicBezierPatch* patch = patches[i][j];
            const Matrix<ifc::Point*>& controlPoints = patch->getPoints();

            int k = 0;
            if(j > 0) k = 1;
            for(; k < CUBIC_COUNT; k++){
                vector<ifc::Point*> row = controlPoints[k];
                for(int l = 0; l < CUBIC_COUNT; l++){
                    ifc::Point* point = row[l];

                    if(!(std::find(points[currentRow].begin(),
                                 points[currentRow].end(), point )
                       != points[currentRow].end())){
                        points[currentRow].push_back(point);
                    }
                }
                currentRow++;
            }
        }
    }
    Matrix<ifc::Point*> pointsMatrix(points,
                                     numberOfPointsInColumns,
                                     numberOfPointsInRow);
    return pointsMatrix;
}

void SurfaceC0Cylind::shapeTheCylidner(const Matrix<ifc::Point*>&rowWisePoints){
    for(unsigned int i = 0; i < rowWisePoints.rowCount(); i++){
        const vector<ifc::Point*>& row = rowWisePoints[i];
        unsigned int size = row.size();
        float da = (2*M_PI) / (float)size;
        float a = 0;
        float x,z;
        for(unsigned int j = 0; j < size; j++){
            x = sin(a) * radius;
            z = cos(a) * radius;

            row[j]->moveTo(x, row[j]->getPosition().y, z);

            a += da;
        }
    }
}

BicubicBezierPatch* SurfaceC0Cylind::createPatch(int n, int m,
                                               float patchHeight,
                                               const vec3& origin){
    float dY = patchHeight / (float)(CUBIC_COUNT - 1);

    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    Matrix<ifc::Point*> points = initC0Points(n,m);

    float x,y,z;
    x = z = 0;

    for(int i = 0; i < CUBIC_COUNT; i++){ // column
        y = m*patchHeight;
        if(m > 0) y+=dY;

        for(int j = 0; j < CUBIC_COUNT; j++ ){ // row
            if(points[j][i] != NULL) continue;

            points[j][i] = objectFactory.createPoint(createPointName(n,m,j,i));
            allPoints.push_back(points[j][i]);
            components.push_back(points[j][i]);

            points[j][i]->moveTo(origin);
            points[j][i]->move(x, y, z);
            y += dY;
        }
    }
    BicubicBezierPatch* patch = new BicubicBezierPatch(points);

    return patch;
}

Matrix<ifc::Point*> SurfaceC0Cylind::initC0Points(int n, int m){
    Matrix<ifc::Point*> points(CUBIC_COUNT, CUBIC_COUNT, NULL);

    if (n > 0){
        BicubicBezierPatch* patch = patches[n-1][m];
        const Matrix<ifc::Point*>& prevPoints = patch->getPoints();
        points.setColumn(0, prevPoints.getColumn(3));

        if(n == this->n - 1){
            BicubicBezierPatch* patch = patches[0][m];
            const Matrix<ifc::Point*>& prevPoints = patch->getPoints();
            points.setColumn(3, prevPoints.getColumn(0));
        }
    }
    if (m > 0){
        BicubicBezierPatch* patch = patches[n][m-1];
        const Matrix<ifc::Point*>& prevPoints = patch->getPoints();
        points[0] = prevPoints[3];
    }

    return points;
}

std::string SurfaceC0Cylind::createPointName(int patchN, int patchM,
                                           int pointI, int pointJ){
    return this->getName() + "("+ to_string(patchN) + "," + to_string(patchM) +
           ")"
           + "_point"
           + "("+ to_string(pointI) + "," + to_string(pointJ) + ")";


}

void SurfaceC0Cylind::drawCPU(const glm::mat4& VP, const Color& color,
                            float u_min, float u_max,
                            float v_min, float v_max,
                            float du, float dv) {
    setSurfaceColor(color);
    glBegin(GL_POINTS);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            BicubicBezierPatch* patch = patches[i][j];
            drawPatch(patch, VP, u_min, u_max, v_min, v_max, du, dv);
        }
    }
    glEnd();
}
/*
void SurfaceC0Cylind::drawGPU(const glm::mat4& VP, const Color& color,
                            float u_min, float u_max,
                            float v_min, float v_max,
                            float du, float dv){
    int patchCount = n*m;
    int pixelCount = (u_max / du) * (v_max / dv) * (patchCount );

    mat4* xComponents = new mat4[patchCount];
    mat4* yComponents = new mat4[patchCount];
    mat4* zComponents = new mat4[patchCount];

    if(pixelCount > MAX_PIXEL_COUNT){
        delete surfacePixels;
        MAX_PIXEL_COUNT = pixelCount;
        surfacePixels = (vec4*) malloc(MAX_PIXEL_COUNT * sizeof(vec4));
    }

    int id = 0;
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            xComponents[id] = patches[i][j]->getX();
            yComponents[id] = patches[i][j]->getY();
            zComponents[id] = patches[i][j]->getZ();
            id++;
        }
    }
}
*/

void SurfaceC0Cylind::drawGPU(const glm::mat4& VP, const Color& color,
                              float u_min, float u_max,
                              float v_min, float v_max,
                              float du, float dv){
    float du_tmp = du;
    float dv_tmp = dv;

    uDivisionCount = 4;
    vDivisionCount = 4;

    float netDu = 1.0f / (float)(uDivisionCount - 1);
    float netDv = 1.0f / (float)(vDivisionCount - 1);

    int patchPixelCount = ((u_max / netDu) * (v_max / dv) +
            ((v_max / netDv) * (u_max / du))) * 2;

    vector<vec2> parameters(patchPixelCount);
    int i = 0;

    du = netDu;
    float u,v;
    for(u = u_min; u <= u_max; u+=du){
        for(v = v_min; v <= v_max; v+=dv){
            vec2 vec(u,v);
            if(i > patchPixelCount){
                parameters.push_back(vec);
            }else{
                parameters[i] = vec;
            }
            i++;
        }
    }
    du = du_tmp;
    dv = netDv;

    for(v = v_min; v <= v_max; v+=dv){
        for(u = u_min; u <= u_max; u+=du){
            vec2 vec(u,v);
            if(i > patchPixelCount){
                parameters.push_back(vec);
            }else{
                parameters[i] = vec;
            }
            i++;
        }
    }
    patchPixelCount = parameters.size();

    // ---------

    int patchCount = n*m;
    int allPixelCount = patchPixelCount * patchCount;

    vector<mat4> xComponents(patchCount);
    vector<mat4> yComponents(patchCount);
    vector<mat4> zComponents(patchCount);


    if(allPixelCount > MAX_PIXEL_COUNT){
        delete surfacePixels;
        MAX_PIXEL_COUNT = allPixelCount;
        surfacePixels = (vec4*) malloc(MAX_PIXEL_COUNT * sizeof(vec4));
    }

    int id = 0;
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            xComponents[id] = patches[i][j]->getX();
            yComponents[id] = patches[i][j]->getY();
            zComponents[id] = patches[i][j]->getZ();
            id++;
        }
    }

    ifc_gpu::computeBezierSurface(xComponents.data(),
                                  yComponents.data(),
                                  zComponents.data(),
                                  patchCount, parameters.data(),
                                  parameters.size(), surfacePixels,
                                  allPixelCount, &VP);

    setSurfaceColor(color);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PIXEL_COUNT; i++) {
        const vec4& point = surfacePixels[i];
        if(point.w < 0) continue;

        glVertex2f(point.x, point.y);

    }
    glEnd();
}

void SurfaceC0Cylind::drawPatch(const BicubicBezierPatch* patch,
                              const glm::mat4& VP,
                              float u_min, float u_max,
                              float v_min, float v_max,
                              float du, float dv){
    float du_tmp = du;
    float dv_tmp = dv;

    uDivisionCount = 4;
    vDivisionCount = 4;

    du = 1.0f / (float)(uDivisionCount - 1);

    float u,v;
    for(u = u_min; u <= u_max; u+=du){
        for(v = v_min; v <= v_max; v+=dv){
            vec4 Bu = cubicBernsteinVector(u);
            vec4 Bv = cubicBernsteinVector(v);

            float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
            float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
            float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

            vec4 point(x, y, z, 1);
            point = VP * point;
            if(point.w < 0) continue;

            point.x /= point.w;
            point.y /= point.w;
            glVertex2f(point.x, point.y);
        }
    }
    du = du_tmp;
    dv = 1.0f / (float)(vDivisionCount - 1);

    for(v = v_min; v <= v_max; v+=dv){
        for(u = u_min; u <= u_max; u+=du){
            vec4 Bu = cubicBernsteinVector(u);
            vec4 Bv = cubicBernsteinVector(v);

            float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
            float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
            float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

            vec4 point(x, y, z, 1);
            point = VP * point;
            if(point.w < 0) continue;

            point.x /= point.w;
            point.y /= point.w;
            glVertex2f(point.x, point.y);
        }
    }
}

//-----------------------//
//  PROTECTED
//-----------------------//

void SurfaceC0Cylind::draw(const glm::mat4& VP, const Color& color) {
    float u_min = 0;
    float u_max = 1;
    float v_min = 0;
    float v_max = 1;
    float du = 0.001;
    float dv = 0.001;

    if(ifc::RUN_CUDA){
        drawGPU(VP, color, u_min, u_max, v_min, v_max, du, dv);

        //drawCPU(VP, color, u_min, u_max, v_min, v_max, du, dv);
    }else{
        drawCPU(VP, color, u_min, u_max, v_min, v_max, du, dv);
    }
}

void SurfaceC0Cylind::build() {
    vec3 startPos(0,0,0);

    float patchHeight = height/(float)m;

    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            patches[i][j] = createPatch(i, j, patchHeight, startPos);
        }
    }

    Matrix<ifc::Point*> rowWisePoints = getMatrixFormOfAllPatches();
    shapeTheCylidner(rowWisePoints);
}

//-----------------------//
//  PUBLIC
//-----------------------//

void SurfaceC0Cylind::update(){
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            BicubicBezierPatch* patch  = patches[i][j];
            if(patch != NULL){
                patch->update();
            }
        }
    }
}

const std::vector<ifc::Point*>& SurfaceC0Cylind::getAllPoints(){
    return this->allPoints;
}
