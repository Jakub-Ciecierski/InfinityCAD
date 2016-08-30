//
// Created by jakub on 5/2/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface.h>
#include <infinity_cad/math/math.h>
#include <ifc_gpu/surfaces/bezier_surface_gpu.h>
#include <infinity_cad/geometry/polynomials/bernstein_basis.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/settings/settings.h>
#include <infinity_cad/rendering/color/color_settings.h>

using namespace std;
using namespace ifc;
using namespace glm;

int Surface::uDivisionCount = 4;
int Surface::vDivisionCount = 4;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

Surface::Surface(SceneID id, std::string name,
                 int n, int m, SurfaceAxis surfaceAxis) :
        RenderObject(id, name), n(n), m(m), patches(n, m, NULL),
        surfaceAxis(surfaceAxis){
    grabable = false;

    setDrawPolygon(false);

    surfacePixels = (vec4 *) malloc(MAX_PIXEL_COUNT * sizeof(vec4));

    this->trimmingStatus = TrimmingStatuses::TRIM_NONE;
}

Surface::~Surface(){
    delete surfacePixels;
}

//-----------------------//
//  PRIVATE
//-----------------------//

void Surface::trimParameters(std::vector<glm::vec2>& params){
    for(unsigned int i = 0; i < params.size(); i++){
        vec2& v = params[i];
    }
}

glm::vec2 Surface::findTrimmingParameter(const glm::vec2& v){
    for(unsigned int i = 1; i < trimmingParameters.size(); i++){
        vec2& tv1 = trimmingParameters[i-1];
        vec2& tv2 = trimmingParameters[i];
        if(v.x > tv1.x && v.x < tv2.x)
            return tv2;
    }
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
    glLineWidth(this->lineWidth);
    //glBegin(GL_POINTS);
    glBegin(GL_LINES);
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
    if(uDivisionCount_tmp < 3)
        uDivisionCount_tmp = 3;

    int vDivisionCount_tmp = vDivisionCount;
    if(vDivisionCount_tmp < 3)
        vDivisionCount_tmp = 3;

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

}

void Surface::drawPatch(const BicubicBezierPatch* patch,
                              const glm::mat4& VP,
                              float u_min, float u_max,
                              float v_min, float v_max,
                              float du, float dv){
    std::vector<vec4> points;

    float du_tmp = du;
    float dv_tmp = dv;

    int uDivisionCount_tmp = uDivisionCount;
    if(uDivisionCount_tmp == 1) uDivisionCount_tmp++;

    int vDivisionCount_tmp = vDivisionCount;
    if(vDivisionCount_tmp == 1) vDivisionCount_tmp++;

    int index = 0;
    vector<vector<int>> edges;

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

            point.x /= point.w;
            point.y /= point.w;

            points.push_back(point);
            vector<int> edge{index, index+1};
            edges.push_back(edge);
            index++;
        }
        v = v_max;
        vec4 Bu = cubicBernsteinVector(u);
        vec4 Bv = cubicBernsteinVector(v);

        float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
        float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
        float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

        vec4 point(x, y, z, 1);
        point = VP * point;

        point.x /= point.w;
        point.y /= point.w;

        points.push_back(point);
        index++;
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
            point.x /= point.w;
            point.y /= point.w;

            points.push_back(point);
            vector<int> edge{index, index+1};
            edges.push_back(edge);
            index++;
        }
        u = u_max;
        vec4 Bu = cubicBernsteinVector(u);
        vec4 Bv = cubicBernsteinVector(v);

        float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
        float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
        float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

        vec4 point(x, y, z, 1);
        point = VP * point;
        point.x /= point.w;
        point.y /= point.w;

        points.push_back(point);
        index++;
    }
    for(unsigned int i = 1; i < edges.size(); i++){
        int index1 = edges[i][0];
        int index2 = edges[i][1];
        vec4& p1 = points[index1];
        vec4& p2 = points[index2];
        if(p1.w < 0 || p2.w < 0) continue;
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
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
    VPMatrix = &VP;

    float u_min = 0;
    float u_max = 1;
    float v_min = 0;
    float v_max = 1;

    float du;
    float dv;

    int screenLength = 3 * (screenWidth+screenHeight);
    //float maxDist = getMaximumPolygonLength();

    //du = 1.0f / ((float)screenLength * maxDist);
    //dv = 1.0f / ((float)screenLength * maxDist);

    du = 0.03;
    dv = 0.03;

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

glm::vec3 Surface::compute(float u, float v){
    if(u < 0) u = 0;
    if(v < 0) v = 0;
    if(u >= 1.0) u = 0.999f;
    if(v >= 1.0) v = 0.999f;

    int j = floor(u * m);
    int i = floor(v * n);

    float minJ = (float)j / (float)m;
    float maxJ = (float)(j+1) / (float)m;
    float rJ = maxJ - minJ;
    u = (u - minJ) / rJ;

    float minI = (float)i / (float)n;
    float maxI = (float)(i+1) / (float)n;
    float rI = maxI - minI;
    v = (v - minI) / rI;

    const BicubicBezierPatch* patch = patches[i][j];

    vec4 Bu = cubicBernsteinVector(u);
    vec4 Bv = cubicBernsteinVector(v);

    float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
    float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
    float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

    vec3 point(x, y, z);

    return point;
}

glm::vec3 Surface::computeDu(float u, float v){
    if(u < 0) u = 0;
    if(v < 0) v = 0;
    if(u >= 1.0) u = 0.999f;
    if(v >= 1.0) v = 0.999f;

    int j = floor(u * m);
    int i = floor(v * n);

    float minJ = (float)j / (float)m;
    float maxJ = (float)(j+1) / (float)m;
    float rJ = maxJ - minJ;
    u = (u - minJ) / rJ;

    float minI = (float)i / (float)n;
    float maxI = (float)(i+1) / (float)n;
    float rI = maxI - minI;
    v = (v - minI) / rI;

    const BicubicBezierPatch* patch = patches[i][j];

    vec4 Bu = cubicBernsteinDerivative(u);
    vec4 Bv = cubicBernsteinVector(v);

    float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
    float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
    float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

    vec3 point(x, y, z);

    return point;
}

glm::vec3 Surface::computeDuu(float u, float v) {
    if(u < 0) u = 0;
    if(v < 0) v = 0;
    if(u >= 1.0) u = 0.999f;
    if(v >= 1.0) v = 0.999f;

    int j = floor(u * m);
    int i = floor(v * n);

    float minJ = (float)j / (float)m;
    float maxJ = (float)(j+1) / (float)m;
    float rJ = maxJ - minJ;
    u = (u - minJ) / rJ;

    float minI = (float)i / (float)n;
    float maxI = (float)(i+1) / (float)n;
    float rI = maxI - minI;
    v = (v - minI) / rI;

    const BicubicBezierPatch* patch = patches[i][j];

    vec4 Bu = cubicBernsteinSecondDerivative(u);
    vec4 Bv = cubicBernsteinVector(v);

    float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
    float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
    float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

    vec3 point(x, y, z);

    return point;
}

glm::vec3 Surface::computeDuv(float u, float v) {
    if(u < 0) u = 0;
    if(v < 0) v = 0;
    if(u >= 1.0) u = 0.999f;
    if(v >= 1.0) v = 0.999f;

    int j = floor(u * m);
    int i = floor(v * n);

    float minJ = (float)j / (float)m;
    float maxJ = (float)(j+1) / (float)m;
    float rJ = maxJ - minJ;
    u = (u - minJ) / rJ;

    float minI = (float)i / (float)n;
    float maxI = (float)(i+1) / (float)n;
    float rI = maxI - minI;
    v = (v - minI) / rI;

    const BicubicBezierPatch* patch = patches[i][j];

    vec4 Bu = cubicBernsteinDerivative(u);
    vec4 Bv = cubicBernsteinDerivative(v);

    float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
    float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
    float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

    vec3 point(x, y, z);

    return point;
}

glm::vec3 Surface::computeDv(float u, float v){
    if(u < 0) u = 0;
    if(v < 0) v = 0;
    if(u >= 1.0) u = 0.999f;
    if(v >= 1.0) v = 0.999f;

    int j = floor(u * m);
    int i = floor(v * n);

    float minJ = (float)j / (float)m;
    float maxJ = (float)(j+1) / (float)m;
    float rJ = maxJ - minJ;
    u = (u - minJ) / rJ;

    float minI = (float)i / (float)n;
    float maxI = (float)(i+1) / (float)n;
    float rI = maxI - minI;
    v = (v - minI) / rI;

    const BicubicBezierPatch* patch = patches[i][j];

    vec4 Bu = cubicBernsteinVector(u);
    vec4 Bv = cubicBernsteinDerivative(v);

    float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
    float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
    float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

    vec3 point(x, y, z);

    return point;
}


glm::vec3 Surface::computeDvv(float u, float v) {
    if(u < 0) u = 0;
    if(v < 0) v = 0;
    if(u >= 1.0) u = 0.999f;
    if(v >= 1.0) v = 0.999f;

    int j = floor(u * m);
    int i = floor(v * n);

    float minJ = (float)j / (float)m;
    float maxJ = (float)(j+1) / (float)m;
    float rJ = maxJ - minJ;
    u = (u - minJ) / rJ;

    float minI = (float)i / (float)n;
    float maxI = (float)(i+1) / (float)n;
    float rI = maxI - minI;
    v = (v - minI) / rI;

    const BicubicBezierPatch* patch = patches[i][j];

    vec4 Bu = cubicBernsteinVector(u);
    vec4 Bv = cubicBernsteinSecondDerivative(v);

    float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
    float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
    float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

    vec3 point(x, y, z);

    return point;
}

glm::vec3 Surface::computeDvu(float u, float v) {
    if(u < 0) u = 0;
    if(v < 0) v = 0;
    if(u >= 1.0) u = 0.999f;
    if(v >= 1.0) v = 0.999f;

    int j = floor(u * m);
    int i = floor(v * n);

    float minJ = (float)j / (float)m;
    float maxJ = (float)(j+1) / (float)m;
    float rJ = maxJ - minJ;
    u = (u - minJ) / rJ;

    float minI = (float)i / (float)n;
    float maxI = (float)(i+1) / (float)n;
    float rI = maxI - minI;
    v = (v - minI) / rI;

    const BicubicBezierPatch* patch = patches[i][j];

    vec4 Bu = cubicBernsteinDerivative(u);
    vec4 Bv = cubicBernsteinDerivative(v);

    float x = ifc::getMultplicationValue(Bu, patch->getX(), Bv);
    float y = ifc::getMultplicationValue(Bu, patch->getY(), Bv);
    float z = ifc::getMultplicationValue(Bu, patch->getZ(), Bv);

    vec3 point(x, y, z);

    return point;
}


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

const Matrix<ifc::Point*>& Surface::getMatrixPoints(){
    return this->allPointsMatrix;
}

void Surface::setDrawPolygon(bool v){
    this->doDrawPolygon = v;
}

bool Surface::isDrawPolygon(){
    return this->doDrawPolygon;
}

int Surface::getRowPatchCount(){
    return n;
}

int Surface::getColumnPatchCount(){
    return m;
}

void Surface::setTrimming(TrimmingStatuses status){
    this->trimmingStatus = status;
}
void Surface::setTrimming(TrimmingStatuses status,
                          std::vector<glm::vec2>& params){
    this->trimmingStatus = status;
    trimmingParameters = params;
}

PointIndex Surface::getIndex(ifc::Point* point){
    int n = allPointsMatrix.rowCount();
    int m = allPointsMatrix.columnCount();

    // Assume one patch
    PointIndex point_index {0, -1, -1};
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(allPointsMatrix[i][j] == point){
                point_index.row_index = i;
                point_index.column_index = j;
                return point_index;
            }
        }
    }

    return point_index;
}

bool Surface::replacePoint(ifc::Point *src, ifc::Point *dest) {
    for(unsigned int i = 0; i < allPoints.size(); i++){
        if(allPoints[i] == src){
            allPoints[i] = dest;
        }
    }
    for(unsigned int i = 0; i < components.size(); i++){
        if(components[i] == src){
            components[i] = dest;
        }
    }

    for(unsigned int i = 0; i < allPointsMatrix.rowCount(); i++){
        for(unsigned int j = 0; j < allPointsMatrix.columnCount(); j++){
            if(allPointsMatrix[i][j] == src){
                allPointsMatrix[i][j] = dest;
            }
        }
    }

    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            BicubicBezierPatch* patch  = (patches)[i][j];
            Matrix<ifc::Point*>& patchPoints = patch->getPoints();
            for(int x = 0; x < 4; x++){
                for(int y = 0; y < 4; y++){
                    if(patchPoints[x][y] == src){
                        patchPoints[x][y] = dest;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
