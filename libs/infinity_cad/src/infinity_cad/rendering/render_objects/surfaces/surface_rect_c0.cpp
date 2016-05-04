//
// Created by jakub on 5/2/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface_rect_c0.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/settings/settings.h>
#include <infinity_cad/geometry/polynomials/bernstein_basis.h>
#include <infinity_cad/math/math.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

SurfaceRectC0::SurfaceRectC0(SceneID id, std::string name,
                             int n, int m, float width, float height) :
        Surface(id, name, n, m), width(width), height(height),
        patches(n,m, NULL){
    this->build();

    surfacePixels = (vec4 *) malloc(MAX_PIXEL_COUNT * sizeof(vec4));
}

SurfaceRectC0::~SurfaceRectC0(){
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            delete patches[i][j];
        }
    }
}

//-----------------------//
//  PRIVATE
//-----------------------//

BicubicBezierPatch* SurfaceRectC0::createPatch(int n, int m,
                                               float patchWidth,
                                               float patchHeight,
                                               const vec3& origin){
    float dX = patchWidth / (float)(CUBIC_COUNT - 1);
    float dY = patchHeight / (float)(CUBIC_COUNT - 1);

    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    Matrix<ifc::Point*> points = initC0Points(n,m);

    float x,y,z;
    z = 0;
    x = n*patchWidth;

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
        x += dX;
    }
    BicubicBezierPatch* patch = new BicubicBezierPatch(points);

    return patch;
}

Matrix<ifc::Point*> SurfaceRectC0::initC0Points(int n, int m){
    Matrix<ifc::Point*> points(CUBIC_COUNT, CUBIC_COUNT, NULL);

    if (n > 0){
        BicubicBezierPatch* patch = patches[n-1][m];
        const Matrix<ifc::Point*>& prevPoints = patch->getPoints();
        points.setColumn(0, prevPoints.getColumn(3));
    }
    if (m > 0){
        BicubicBezierPatch* patch = patches[n][m-1];
        const Matrix<ifc::Point*>& prevPoints = patch->getPoints();
        points[0] = prevPoints[3];
    }

    return points;
}

std::string SurfaceRectC0::createPointName(int patchN, int patchM,
                                           int pointI, int pointJ){
    return this->getName() + "("+ to_string(patchN) + "," + to_string(patchM) +
            ")"
    + "_point"
    + "("+ to_string(pointI) + "," + to_string(pointJ) + ")";


}

void SurfaceRectC0::drawCPU(const glm::mat4& VP, const Color& color,
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

void SurfaceRectC0::drawGPU(const glm::mat4& VP, const Color& color,
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

void SurfaceRectC0::drawPatch(const BicubicBezierPatch* patch,
                              const glm::mat4& VP,
                              float u_min, float u_max,
                              float v_min, float v_max,
                              float du, float dv){
    float du_tmp = du;
    float dv_tmp = dv;

    du = 0.25;
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
    dv = 0.25;

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

void SurfaceRectC0::draw(const glm::mat4& VP, const Color& color) {
    float u_min = 0;
    float u_max = 1;
    float v_min = 0;
    float v_max = 1;
    float du = 0.001;
    float dv = 0.001;

    if(ifc::RUN_CUDA){
        drawGPU(VP, color, u_min, u_max, v_min, v_max, du, dv);
    }else{
        drawCPU(VP, color, u_min, u_max, v_min, v_max, du, dv);
    }
}

void SurfaceRectC0::build() {
    vec3 startPos(0,0,0);

    float patchWidth = width/(float)n;
    float patchHeight = height/(float)m;

    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            patches[i][j] = createPatch(i,j, patchWidth, patchHeight, startPos);
        }
    }
}

//-----------------------//
//  PUBLIC 
//-----------------------//

void SurfaceRectC0::update(){
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            BicubicBezierPatch* patch  = patches[i][j];
            if(patch != NULL){
                patch->update();
            }
        }
    }

}

const std::vector<ifc::Point*>& SurfaceRectC0::getAllPoints(){
    return this->allPoints;
}
