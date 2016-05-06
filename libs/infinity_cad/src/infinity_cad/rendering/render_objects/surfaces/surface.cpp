//
// Created by jakub on 5/2/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface.h>
#include <infinity_cad/math/math.h>
#include <ifc_gpu/surfaces/bezier_surface_gpu.h>
#include <infinity_cad/geometry/polynomials/bernstein_basis.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/settings/settings.h>

using namespace std;
using namespace ifc;
using namespace glm;

int Surface::uDivisionCount = 4;
int Surface::vDivisionCount = 4;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Surface::Surface(SceneID id, std::string name,
                 int n, int m) :
        RenderObject(id, name), n(n), m(m), patches(n, m, NULL) {
    grabable = false;

    setDrawPolygon(false);

    surfacePixels = (vec4 *) malloc(MAX_PIXEL_COUNT * sizeof(vec4));

    surfacePixelsTMP = NULL;
}

Surface::~Surface(){
    delete surfacePixels;
}


//-----------------------//
//  PROTECTED
//-----------------------//

void Surface::initVertices(){

}

void Surface::initEdges() {

}

std::string Surface::createPointName(int patchN, int patchM,
                                           int pointI, int pointJ){
    return this->getName() + "("+ to_string(patchN) + "," + to_string(patchM) +
           ")"
           + "_point"
           + "("+ to_string(pointI) + "," + to_string(pointJ) + ")";


}

void Surface::drawCPU(const glm::mat4& VP, const Color& color,
                            float u_min, float u_max,
                            float v_min, float v_max,
                            float du, float dv) {
    setSurfaceColor(color);
    glBegin(GL_POINTS);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            BicubicBezierPatch* patch = (patches)[i][j];
            drawPatch(patch, VP, u_min, u_max, v_min, v_max, du, dv);
        }
    }
    glEnd();
}

void Surface::drawGPU(const glm::mat4& VP, const Color& color,
                            float u_min, float u_max,
                            float v_min, float v_max,
                            float du, float dv){
    float du_tmp = du;
    float dv_tmp = dv;

    int uDivisionCount_tmp = uDivisionCount;
    if(uDivisionCount_tmp == 1) uDivisionCount_tmp++;

    int vDivisionCount_tmp = vDivisionCount;
    if(vDivisionCount_tmp == 1) vDivisionCount_tmp++;

    float netDu = 1.0f / (float)(uDivisionCount_tmp - 1);
    float netDv = 1.0f / (float)(vDivisionCount_tmp - 1);

    int patchPixelCount = ((u_max / netDu) * (v_max / dv) +
                           ((v_max / netDv) * (u_max / du))) * 2;

    vector<vec2> parameters(patchPixelCount);
    vector<vector<int>> edges(patchPixelCount);

    int i = 0;
    int edgeID = 0;
    int edgeIndex = 0;

    du = netDu;
    float u,v;

    u = u_min;
    while(u <= u_max){
        v = v_min;
        while(v < v_max){
            vec2 vec(u,v);
            if(i > patchPixelCount){
                parameters.push_back(vec);
            }else{
                parameters[i] = vec;
            }
            vector<int> edge = {edgeID, edgeID + 1};
            if(edgeIndex > patchPixelCount){
                edges.push_back(edge);
            }else{
                edges[edgeIndex++] = edge;
            }


            i++;
            edgeID++;
            v+=dv;

            if(v > v_max){
                v = v_max;
                vec2 vec(u,v);
                if(i > patchPixelCount){
                    parameters.push_back(vec);
                }else{
                    parameters[i] = vec;
                }
                i++;
            }
        }
        edgeID++;
        u+=du;
    }

    du = du_tmp;
    dv = netDv;
    v = v_min;
    while(v <= v_max){
        u = u_min;
        while(u < u_max){
            vec2 vec(u,v);
            if(i > patchPixelCount){
                parameters.push_back(vec);
            }else{
                parameters[i] = vec;
            }
            vector<int> edge = {edgeID, edgeID + 1};
            if(edgeIndex > patchPixelCount){
                edges.push_back(edge);
            }else{
                edges[edgeIndex++] = edge;
            }

            i++;
            edgeID++;
            u+=du;

            if(u > u_max){
                u = u_max;
                vec2 vec(u,v);
                if(i > patchPixelCount){
                    parameters.push_back(vec);
                }else{
                    parameters[i] = vec;
                }
                i++;
            }
        }
        edgeID++;
        v+=dv;
    }

    patchPixelCount = i;
    int edgesCount = edgeIndex;

    // ---------

    int patchCount = n*m;
    int allPixelCount = patchPixelCount * patchCount;

    vector<mat4> xComponents(patchCount);
    vector<mat4> yComponents(patchCount);
    vector<mat4> zComponents(patchCount);

    MAX_PIXEL_COUNT = allPixelCount;
    delete surfacePixels;
    surfacePixels = (vec4*) malloc(MAX_PIXEL_COUNT * sizeof(vec4));

    int id = 0;
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            xComponents[id] = (patches)[i][j]->getX();
            yComponents[id] = (patches)[i][j]->getY();
            zComponents[id] = (patches)[i][j]->getZ();
            id++;
        }
    }

    ifc_gpu::computeBezierSurface(xComponents.data(),
                                  yComponents.data(),
                                  zComponents.data(),
                                  patchCount, parameters.data(),
                                  patchPixelCount, surfacePixels,
                                  allPixelCount, &VP);



    setSurfaceColor(color);
    /*
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PIXEL_COUNT; i++) {
        const vec4& point = surfacePixels[i];
        if(point.w < 0) continue;

        glVertex2f(point.x, point.y);

    }*/


    glLineWidth((GLfloat)lineWidth);
    glBegin(GL_LINES);
    for(int patchIndex = 0; patchIndex < patchCount; patchIndex++){
        for(unsigned int i = 0; i < edgesCount; i++){
            int p1 = edges[i][0];
            int p2 = edges[i][1];

            int globalIndex1 = patchIndex*patchPixelCount + p1;
            int globalIndex2 = patchIndex*patchPixelCount + p2;

            const vec4& point1 = surfacePixels[globalIndex1];
            const vec4& point2 = surfacePixels[globalIndex2];
            if(point1.w < 0 || point2.w < 0) continue;

            glVertex2f(point1.x, point1.y);
            glVertex2f(point2.x, point2.y);
        }
    }
    glEnd();


/*

    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);
    glBegin(GL_LINES);
    for(int patchIndex = 0; patchIndex < patchCount; patchIndex++){
        if(surfacePixels != NULL){
            delete surfacePixels;
            surfacePixels = (vec4*) malloc(patchPixelCount * sizeof(vec4));
        }

        ifc_gpu::computeBezierSurface(&xComponents[patchIndex],
                                      &yComponents[patchIndex],
                                      &zComponents[patchIndex],
                                      1, parameters.data(), parameters.size(),
                                      surfacePixels, patchPixelCount, &VP);

        for(unsigned int i = 0; i < edges.size(); i++){
            int p1 = edges[i][0];
            int p2 = edges[i][1];

            const vec4& point1 = surfacePixels[p1];
            const vec4& point2 = surfacePixels[p2];
            if(point1.w < 0 || point2.w < 0) continue;

            glVertex2f(point1.x, point1.y);
            glVertex2f(point2.x, point2.y);
        }
    }

    glEnd();*/

}

void Surface::drawPatch(const BicubicBezierPatch* patch,
                              const glm::mat4& VP,
                              float u_min, float u_max,
                              float v_min, float v_max,
                              float du, float dv){
    float du_tmp = du;
    float dv_tmp = dv;

    int uDivisionCount_tmp = uDivisionCount;
    if(uDivisionCount_tmp == 1) uDivisionCount_tmp++;

    int vDivisionCount_tmp = vDivisionCount;
    if(vDivisionCount_tmp == 1) vDivisionCount_tmp++;

    du = 1.0f / (float)(uDivisionCount_tmp - 1);

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
    dv = 1.0f / (float)(vDivisionCount_tmp - 1);

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

void Surface::drawPolygon(const glm::mat4& VP, int segments){
    Color color(0.6,0.6,0.6,1);
    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);

    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            BicubicBezierPatch* patch  = (patches)[i][j];
            const Matrix<ifc::Point*>& matrixPoints = patch->getPoints();

            for(int i = 0; i < CUBIC_COUNT; i++){
                std::vector<ifc::Point*> row = matrixPoints[i];

                for(unsigned int j = 0; j < row.size() - 1; j++){
                    ObjectFactory& objectFactory = ObjectFactory::getInstance();

                    const ifc::Point* point1 = row[j];
                    const ifc::Point* point2 = row[j+1];

                    const vec3& pos1 = point1->getPosition();
                    const vec3& pos2 = point2->getPosition();

                    vec4 v1 = vec4(pos1.x, pos1.y, pos1.z, 1);
                    vec4 v2 = vec4(pos2.x, pos2.y, pos2.z, 1);

                    float segDelta = 1 / (float)segments;
                    float currSeg = 0;

                    while(currSeg <= 1){
                        vec4 v = v1 + currSeg*(v2-v1);
                        currSeg += segDelta;

                        Line* line = objectFactory.createLine("line", v1, v);
                        line->update();
                        line->render(VP, color);
                        delete line;
                    }

                    // clipping hack TODO
                    currSeg = 0;
                    while(currSeg <= 1){
                        vec4 v = v2 + currSeg*(v1-v2);
                        currSeg += segDelta;

                        Line* line = objectFactory.createLine("line", v, v2);
                        line->update();
                        line->render(VP, color);
                        delete line;
                    }

                }
            }

            for(int i = 0; i < CUBIC_COUNT; i++){
                std::vector<ifc::Point*> column = matrixPoints.getColumn(i);

                for(unsigned int j = 0; j < column.size() - 1; j++){
                    ObjectFactory& objectFactory = ObjectFactory::getInstance();

                    const ifc::Point* point1 = column[j];
                    const ifc::Point* point2 = column[j+1];

                    const vec3& pos1 = point1->getPosition();
                    const vec3& pos2 = point2->getPosition();

                    vec4 v1 = vec4(pos1.x, pos1.y, pos1.z, 1);
                    vec4 v2 = vec4(pos2.x, pos2.y, pos2.z, 1);

                    float segDelta = 1 / (float)segments;
                    float currSeg = 0;

                    while(currSeg <= 1){
                        vec4 v = v1 + currSeg*(v2-v1);
                        currSeg += segDelta;

                        Line* line = objectFactory.createLine("line", v1, v);
                        line->update();
                        line->render(VP, color);
                        delete line;
                    }

                    // clipping hack TODO
                    currSeg = 0;
                    while(currSeg <= 1){
                        vec4 v = v2 + currSeg*(v1-v2);
                        currSeg += segDelta;

                        Line* line = objectFactory.createLine("line", v, v2);
                        line->update();
                        line->render(VP, color);
                        delete line;
                    }

                }
            }
        }
    }

    glBegin(GL_LINES);
}

float Surface::getMaximumPolygonLength() {
    float maximumSumOfDist = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            BicubicBezierPatch *patch = (patches)[i][j];
            const Matrix<ifc::Point *> &matrixPoints = patch->getPoints();

            for (int i = 0; i < CUBIC_COUNT; i++) {
                float sumOfDist = 0;
                std::vector<ifc::Point *> row = matrixPoints[i];
                for(unsigned int j = 0; j < row.size() - 1; j++){
                    const ifc::Point* point1 = row[j];
                    const ifc::Point* point2 = row[j+1];

                    sumOfDist += ifc::euclideanDistance(point1->getPosition(),
                                                        point2->getPosition());
                }
                if(sumOfDist > maximumSumOfDist){
                    maximumSumOfDist = sumOfDist;
                }
            }

            for (int i = 0; i < CUBIC_COUNT; i++) {
                float sumOfDist = 0;
                std::vector<ifc::Point *> column = matrixPoints.getColumn(i);
                for(unsigned int j = 0; j < column.size() - 1; j++){
                    const ifc::Point* point1 = column[j];
                    const ifc::Point* point2 = column[j+1];

                    sumOfDist += ifc::euclideanDistance(point1->getPosition(),
                                                        point2->getPosition());
                }
                if(sumOfDist > maximumSumOfDist){
                    maximumSumOfDist = sumOfDist;
                }
            }
        }
    }

    return maximumSumOfDist;
}

void Surface::draw(const glm::mat4& VP, const Color& color) {
    float u_min = 0;
    float u_max = 1;
    float v_min = 0;
    float v_max = 1;

    float du;
    float dv;

    int screenLength = 3 * (screenWidth+screenHeight);
    float maxDist = getMaximumPolygonLength();

    //du = 1.0f / ((float)screenLength * maxDist);
    //dv = 1.0f / ((float)screenLength * maxDist);

    du = 0.06;
    dv = 0.06;

    if(ifc::RUN_CUDA){
        drawGPU(VP, color, u_min, u_max, v_min, v_max, du, dv);
    }else{
        drawCPU(VP, color, u_min, u_max, v_min, v_max, du, dv);
    }

    if(doDrawPolygon){
        drawPolygon(VP);
    }
}

//-----------------------//
//  PUBLIC
//-----------------------//


void Surface::render(const glm::mat4 &VP) {
    draw(VP, color);
}

void Surface::render(const glm::mat4 &VP, const Color &color) {
    draw(VP, color);
}

void Surface::update(){
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            BicubicBezierPatch* patch  = (patches)[i][j];
            if(patch != NULL){
                patch->update();
            }
        }
    }

}

const std::vector<ifc::Point*>& Surface::getAllPoints(){
    return this->allPoints;
}

void Surface::setDrawPolygon(bool v){
    this->doDrawPolygon = v;
}

bool Surface::isDrawPolygon(){
    return this->doDrawPolygon;
}
