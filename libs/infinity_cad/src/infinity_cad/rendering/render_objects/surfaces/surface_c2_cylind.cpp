//
// Created by jakub on 5/15/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface_c2_cylind.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <algorithm>
#include <infinity_cad/geometry/polynomials/bspline_basis.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

SurfaceC2Cylind::SurfaceC2Cylind(SceneID id, std::string name,
                                 int n, int m,
                                 float radius, float height,
                                 SurfaceAxis surfaceAxis) :
        Surface(id, name, n, m, surfaceAxis),
        radius(radius), height(height){
    build();
    update();

    objectType = OBJ_TYPE_SURFACE_BSPLINE_CYLIND;

    matrixMajor = MatrixMajor::ROW;
}

SurfaceC2Cylind::SurfaceC2Cylind(SceneID id, std::string name,
                                 Matrix<ifc::Point*> points) :
        Surface(id, name, 0, 0){
    int rowCount = points.rowCount();
    int columnCount = points.columnCount();

    n = rowCount;
    m = columnCount - 3;

    int allPointsRowCount = rowCount + 3;
    int allPointsColumnCount = columnCount;

    patches = Matrix<BicubicBezierPatch*>(n, m, NULL);

    deboorPoints = new Matrix<ifc::Point*>(allPointsRowCount,
                                           allPointsColumnCount);

    for(int i = 0; i < allPointsRowCount - 3; i++) {
        for (int j = 0; j < allPointsColumnCount; j++) {
            deboorPoints->setEntry(i, j, points[i][j]);
            allPoints.push_back(points[i][j]);
            components.push_back(points[i][j]);
        }
    }
    vector<ifc::Point*> fistColumn = (*deboorPoints)[0];
    vector<ifc::Point*> secondColumn = (*deboorPoints)[1];
    vector<ifc::Point*> thirdColumn = (*deboorPoints)[2];

    (*deboorPoints)[allPointsRowCount-1] = thirdColumn;
    (*deboorPoints)[allPointsRowCount-2] = secondColumn;
    (*deboorPoints)[allPointsRowCount-3] = fistColumn;

    allPointsMatrix = *deboorPoints;

    allPointsMatrix.resize(allPointsRowCount - 3, allPointsColumnCount);
    allPointsMatrix = points;

    update();

    objectType = OBJ_TYPE_SURFACE_BSPLINE_CYLIND;
    matrixMajor = MatrixMajor::COLUMN;
}

SurfaceC2Cylind::~SurfaceC2Cylind(){
    delete deboorPoints;
}

//-----------------------//
//  PROTECTED
//-----------------------//

void SurfaceC2Cylind::build(){
    vec3 origin(0,0,0);

    int rowCount = n + 3;
    int columnCount = m + 3;
    deboorPoints = new Matrix<ifc::Point*>(rowCount, columnCount);

    float x,y,z;
    x = y = z = 0;

    float dy = height / (float)columnCount;

    float M_2PI = M_PI*2;
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    for(int i = 0; i < rowCount; i++){
        for(int j = 0; j < columnCount-3; j++){
            (*deboorPoints)[i][j]
                    = objectFactory.createPoint(createPointName(n,m,i,j));

            allPoints.push_back((*deboorPoints)[i][j]);
            components.push_back((*deboorPoints)[i][j]);

            if(surfaceAxis == SurfaceAxis::HORIZONTAL)
                z = radius * cos(M_2PI * j / (columnCount-3));
            if(surfaceAxis == SurfaceAxis::VERTICAL)
                y = radius * cos(M_2PI * j / (columnCount-3));
            x = radius * sin(M_2PI * j / (columnCount-3));

            (*deboorPoints)[i][j]->moveTo(origin);
            (*deboorPoints)[i][j]->move(x, y, z);
        }
        if(surfaceAxis == SurfaceAxis::HORIZONTAL)
            y += dy;
        if(surfaceAxis == SurfaceAxis::VERTICAL)
            z += dy;
    }
    vector<ifc::Point*> fistColumn = deboorPoints->getColumn(0);
    vector<ifc::Point*> secondColumn = deboorPoints->getColumn(1);
    vector<ifc::Point*> thirdColumn = deboorPoints->getColumn(2);

    deboorPoints->setColumn(columnCount-1, thirdColumn);
    deboorPoints->setColumn(columnCount-2, secondColumn);
    deboorPoints->setColumn(columnCount-3, fistColumn);

    // for serialization
    allPointsMatrix = *deboorPoints;
    allPointsMatrix.resize(deboorPoints->rowCount(),
                           deboorPoints->columnCount() - 3);
}
/*
void SurfaceC2Cylind::build(){
    vec3 origin(0,0,0);

    int rowCount = n + 3;
    int columnCount = m + 3;
    deboorPoints = new Matrix<ifc::Point*>(rowCount, columnCount);

    float x,y,z;
    x = y = z = 0;

    float dy = height / (float)columnCount;

    float M_2PI = M_PI*2;
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    for(int i = 0; i < rowCount; i++){
        for(int j = 0; j < columnCount-3; j++){
            (*deboorPoints)[i][j]
                    = objectFactory.createPoint(createPointName(n,m,i,j));

            allPoints.push_back((*deboorPoints)[i][j]);
            components.push_back((*deboorPoints)[i][j]);

            if(surfaceAxis == SurfaceAxis::HORIZONTAL)
                z = radius * cos(M_2PI * j / (columnCount-3));
            if(surfaceAxis == SurfaceAxis::VERTICAL)
                y = radius * cos(M_2PI * j / (columnCount-3));
            x = radius * sin(M_2PI * j / (columnCount-3));

            (*deboorPoints)[i][j]->moveTo(origin);
            (*deboorPoints)[i][j]->move(x, y, z);
        }
        if(surfaceAxis == SurfaceAxis::HORIZONTAL)
            y += dy;
        if(surfaceAxis == SurfaceAxis::VERTICAL)
            z += dy;
    }
    vector<ifc::Point*> fistColumn = deboorPoints->getColumn(0);
    vector<ifc::Point*> secondColumn = deboorPoints->getColumn(1);
    vector<ifc::Point*> thirdColumn = deboorPoints->getColumn(2);

    deboorPoints->setColumn(columnCount-1, thirdColumn);
    deboorPoints->setColumn(columnCount-2, secondColumn);
    deboorPoints->setColumn(columnCount-3, fistColumn);

    // for serialization
    allPointsMatrix = *deboorPoints;
    allPointsMatrix.resize(deboorPoints->rowCount(),
                           deboorPoints->columnCount() - 3);
}
*/
//-----------------------//
//  PUBLIC
//-----------------------//

glm::vec3 SurfaceC2Cylind::compute(float u, float v) {
    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    const int DEGREE = 3;
    int n = deboorPoints->rowCount();
    int m = deboorPoints->columnCount();

    int d = DEGREE;
    int knotCountU = n + d + 1;
    int knotCountV = m + d + 1;

    //float du = 1.0f / (float)(knotCountU - 2*DEGREE - 1);
    //float dv = 1.0f / (float)(knotCountV - 2*DEGREE - 1);
    float du = 1.0f / (float)(knotCountU - 1);
    float dv = 1.0f / (float)(knotCountV - 1);
    float uknot = 0;
    float vknot = 0;
    for(int i = 0; i < knotCountU; i++){
        knotVectorU.push_back(uknot);
        //if(i > 2 && i < (knotCountU - 1 - 3)) uknot += du;
        uknot += du;
    }
    for(int i = 0; i < knotCountV; i++){
        knotVectorV.push_back(vknot );
        //if(i > 2 && i < (knotCountV - 1 - 3)) vknot += dv;
        vknot += dv;
    }
    //float umin = knotVectorU[4];
    float umin = knotVectorU[3];
    //float umax = knotVectorU[knotCountU - 1 - 4];
    float umax = knotVectorU[knotCountU - 1 - 3];
    float ru = umax - umin;
    u = ru * u + umin;

    //float vmin = knotVectorV[4];
    float vmin = knotVectorV[3];
    //float vmax = knotVectorV[knotCountU - 1 - 4];
    float vmax = knotVectorV[knotCountV - 1 - 3];
    float rv = vmax - vmin;
    v = rv * v + vmin;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < deboorPoints->rowCount(); i++){
        for(int j = 0; j < deboorPoints->columnCount(); j++){
            float bsU = bsplineRecurive(u, DEGREE, i, knotVectorU);
            float bsV = bsplineRecurive(v, DEGREE, j, knotVectorV);
            ifc::Point* point = (*deboorPoints)[i][j];
            const vec3& pos = point->getPosition();
            result += bsU * bsV * pos;
        }
    }
    return result;
}


glm::vec3 SurfaceC2Cylind::computeDu(float u, float v){
    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    const int DEGREE = 3;
    int n = deboorPoints->rowCount();
    int m = deboorPoints->columnCount();

    int d = DEGREE;
    int knotCountU = n + d + 1;
    int knotCountV = m + d + 1;

    //float du = 1.0f / (float)(knotCountU - 2*DEGREE - 1);
    //float dv = 1.0f / (float)(knotCountV - 2*DEGREE - 1);
    float du = 1.0f / (float)(knotCountU - 1);
    float dv = 1.0f / (float)(knotCountV - 1);
    float uknot = 0;
    float vknot = 0;
    for(int i = 0; i < knotCountU; i++){
        knotVectorU.push_back(uknot);
        //if(i > 2 && i < (knotCountU - 1 - 3)) uknot += du;
        uknot += du;
    }
    for(int i = 0; i < knotCountV; i++){
        knotVectorV.push_back(vknot );
        //if(i > 2 && i < (knotCountV - 1 - 3)) vknot += dv;
        vknot += dv;
    }
    //float umin = knotVectorU[4];
    float umin = knotVectorU[3];
    //float umax = knotVectorU[knotCountU - 1 - 4];
    float umax = knotVectorU[knotCountU - 1 - 3];
    float ru = umax - umin;
    u = ru * u + umin;

    //float vmin = knotVectorV[4];
    float vmin = knotVectorV[3];
    //float vmax = knotVectorV[knotCountU - 1 - 4];
    float vmax = knotVectorV[knotCountV - 1 - 3];
    float rv = vmax - vmin;
    v = rv * v + vmin;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < deboorPoints->rowCount(); i++){
        for(int j = 0; j < deboorPoints->columnCount(); j++){
            float bsU = bsplineDerivativeRecurive(u, DEGREE, i, knotVectorU);
            float bsV = bsplineRecurive(v, DEGREE, j, knotVectorV);
            ifc::Point* point = (*deboorPoints)[i][j];
            const vec3& pos = point->getPosition();
            result += bsU * bsV * pos;
        }
    }
    return result;
}

glm::vec3 SurfaceC2Cylind::computeDuu(float u, float v){
    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    const int DEGREE = 3;
    int n = deboorPoints->rowCount();
    int m = deboorPoints->columnCount();

    int d = DEGREE;
    int knotCountU = n + d + 1;
    int knotCountV = m + d + 1;

    //float du = 1.0f / (float)(knotCountU - 2*DEGREE - 1);
    //float dv = 1.0f / (float)(knotCountV - 2*DEGREE - 1);
    float du = 1.0f / (float)(knotCountU - 1);
    float dv = 1.0f / (float)(knotCountV - 1);
    float uknot = 0;
    float vknot = 0;
    for(int i = 0; i < knotCountU; i++){
        knotVectorU.push_back(uknot);
        //if(i > 2 && i < (knotCountU - 1 - 3)) uknot += du;
        uknot += du;
    }
    for(int i = 0; i < knotCountV; i++){
        knotVectorV.push_back(vknot );
        //if(i > 2 && i < (knotCountV - 1 - 3)) vknot += dv;
        vknot += dv;
    }
    //float umin = knotVectorU[4];
    float umin = knotVectorU[3];
    //float umax = knotVectorU[knotCountU - 1 - 4];
    float umax = knotVectorU[knotCountU - 1 - 3];
    float ru = umax - umin;
    u = ru * u + umin;

    //float vmin = knotVectorV[4];
    float vmin = knotVectorV[3];
    //float vmax = knotVectorV[knotCountU - 1 - 4];
    float vmax = knotVectorV[knotCountV - 1 - 3];
    float rv = vmax - vmin;
    v = rv * v + vmin;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < deboorPoints->rowCount(); i++){
        for(int j = 0; j < deboorPoints->columnCount(); j++){
            float bsU = bsplineDerivativeRecurive(u, DEGREE, i, knotVectorU);
            float bsUU = bsplineDerivativeRecurive(bsU, DEGREE, i, knotVectorU);
            float bsV = bsplineRecurive(v, DEGREE, j, knotVectorV);
            ifc::Point* point = (*deboorPoints)[i][j];
            const vec3& pos = point->getPosition();
            result += bsUU * bsV * pos;
        }
    }
    return result;
}

glm::vec3 SurfaceC2Cylind::computeDuv(float u, float v){
    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    const int DEGREE = 3;
    int n = deboorPoints->rowCount();
    int m = deboorPoints->columnCount();

    int d = DEGREE;
    int knotCountU = n + d + 1;
    int knotCountV = m + d + 1;

    //float du = 1.0f / (float)(knotCountU - 2*DEGREE - 1);
    //float dv = 1.0f / (float)(knotCountV - 2*DEGREE - 1);
    float du = 1.0f / (float)(knotCountU - 1);
    float dv = 1.0f / (float)(knotCountV - 1);
    float uknot = 0;
    float vknot = 0;
    for(int i = 0; i < knotCountU; i++){
        knotVectorU.push_back(uknot);
        //if(i > 2 && i < (knotCountU - 1 - 3)) uknot += du;
        uknot += du;
    }
    for(int i = 0; i < knotCountV; i++){
        knotVectorV.push_back(vknot );
        //if(i > 2 && i < (knotCountV - 1 - 3)) vknot += dv;
        vknot += dv;
    }
    //float umin = knotVectorU[4];
    float umin = knotVectorU[3];
    //float umax = knotVectorU[knotCountU - 1 - 4];
    float umax = knotVectorU[knotCountU - 1 - 3];
    float ru = umax - umin;
    u = ru * u + umin;

    //float vmin = knotVectorV[4];
    float vmin = knotVectorV[3];
    //float vmax = knotVectorV[knotCountU - 1 - 4];
    float vmax = knotVectorV[knotCountV - 1 - 3];
    float rv = vmax - vmin;
    v = rv * v + vmin;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < deboorPoints->rowCount(); i++){
        for(int j = 0; j < deboorPoints->columnCount(); j++){
            float bsU = bsplineDerivativeRecurive(u, DEGREE, i, knotVectorU);
            float bsV = bsplineDerivativeRecurive(v, DEGREE, j, knotVectorV);
            ifc::Point* point = (*deboorPoints)[i][j];
            const vec3& pos = point->getPosition();
            result += bsU * bsV * pos;
        }
    }
    return result;
}

glm::vec3 SurfaceC2Cylind::computeDv(float u, float v){
    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    const int DEGREE = 3;
    int n = deboorPoints->rowCount();
    int m = deboorPoints->columnCount();

    int d = DEGREE;
    int knotCountU = n + d + 1;
    int knotCountV = m + d + 1;

    //float du = 1.0f / (float)(knotCountU - 2*DEGREE - 1);
    //float dv = 1.0f / (float)(knotCountV - 2*DEGREE - 1);
    float du = 1.0f / (float)(knotCountU - 1);
    float dv = 1.0f / (float)(knotCountV - 1);
    float uknot = 0;
    float vknot = 0;
    for(int i = 0; i < knotCountU; i++){
        knotVectorU.push_back(uknot);
        //if(i > 2 && i < (knotCountU - 1 - 3)) uknot += du;
        uknot += du;
    }
    for(int i = 0; i < knotCountV; i++){
        knotVectorV.push_back(vknot );
        //if(i > 2 && i < (knotCountV - 1 - 3)) vknot += dv;
        vknot += dv;
    }
    //float umin = knotVectorU[4];
    float umin = knotVectorU[3];
    //float umax = knotVectorU[knotCountU - 1 - 4];
    float umax = knotVectorU[knotCountU - 1 - 3];
    float ru = umax - umin;
    u = ru * u + umin;

    //float vmin = knotVectorV[4];
    float vmin = knotVectorV[3];
    //float vmax = knotVectorV[knotCountU - 1 - 4];
    float vmax = knotVectorV[knotCountV - 1 - 3];
    float rv = vmax - vmin;
    v = rv * v + vmin;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < deboorPoints->rowCount(); i++){
        for(int j = 0; j < deboorPoints->columnCount(); j++){
            float bsU = bsplineRecurive(u, DEGREE, i, knotVectorU);
            float bsV = bsplineDerivativeRecurive(v, DEGREE, j, knotVectorV);
            ifc::Point* point = (*deboorPoints)[i][j];
            const vec3& pos = point->getPosition();
            result += bsU * bsV * pos;
        }
    }
    return result;
}

glm::vec3 SurfaceC2Cylind::computeDvv(float u, float v){
    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    const int DEGREE = 3;
    int n = deboorPoints->rowCount();
    int m = deboorPoints->columnCount();

    int d = DEGREE;
    int knotCountU = n + d + 1;
    int knotCountV = m + d + 1;

    //float du = 1.0f / (float)(knotCountU - 2*DEGREE - 1);
    //float dv = 1.0f / (float)(knotCountV - 2*DEGREE - 1);
    float du = 1.0f / (float)(knotCountU - 1);
    float dv = 1.0f / (float)(knotCountV - 1);
    float uknot = 0;
    float vknot = 0;
    for(int i = 0; i < knotCountU; i++){
        knotVectorU.push_back(uknot);
        //if(i > 2 && i < (knotCountU - 1 - 3)) uknot += du;
        uknot += du;
    }
    for(int i = 0; i < knotCountV; i++){
        knotVectorV.push_back(vknot );
        //if(i > 2 && i < (knotCountV - 1 - 3)) vknot += dv;
        vknot += dv;
    }
    //float umin = knotVectorU[4];
    float umin = knotVectorU[3];
    //float umax = knotVectorU[knotCountU - 1 - 4];
    float umax = knotVectorU[knotCountU - 1 - 3];
    float ru = umax - umin;
    u = ru * u + umin;

    //float vmin = knotVectorV[4];
    float vmin = knotVectorV[3];
    //float vmax = knotVectorV[knotCountU - 1 - 4];
    float vmax = knotVectorV[knotCountV - 1 - 3];
    float rv = vmax - vmin;
    v = rv * v + vmin;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < deboorPoints->rowCount(); i++){
        for(int j = 0; j < deboorPoints->columnCount(); j++){

            float bsU = bsplineRecurive(u, DEGREE, i, knotVectorU);
            float bsV = bsplineDerivativeRecurive(v, DEGREE, j, knotVectorV);
            float bsVV = bsplineDerivativeRecurive(bsV, DEGREE, j, knotVectorV);
            ifc::Point* point = (*deboorPoints)[i][j];
            const vec3& pos = point->getPosition();
            result += bsU * bsVV * pos;
        }
    }
    return result;
}

glm::vec3 SurfaceC2Cylind::computeDvu(float u, float v){
    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    const int DEGREE = 3;
    int n = deboorPoints->rowCount();
    int m = deboorPoints->columnCount();

    int d = DEGREE;
    int knotCountU = n + d + 1;
    int knotCountV = m + d + 1;

    //float du = 1.0f / (float)(knotCountU - 2*DEGREE - 1);
    //float dv = 1.0f / (float)(knotCountV - 2*DEGREE - 1);
    float du = 1.0f / (float)(knotCountU - 1);
    float dv = 1.0f / (float)(knotCountV - 1);
    float uknot = 0;
    float vknot = 0;
    for(int i = 0; i < knotCountU; i++){
        knotVectorU.push_back(uknot);
        //if(i > 2 && i < (knotCountU - 1 - 3)) uknot += du;
        uknot += du;
    }
    for(int i = 0; i < knotCountV; i++){
        knotVectorV.push_back(vknot );
        //if(i > 2 && i < (knotCountV - 1 - 3)) vknot += dv;
        vknot += dv;
    }
    //float umin = knotVectorU[4];
    float umin = knotVectorU[3];
    //float umax = knotVectorU[knotCountU - 1 - 4];
    float umax = knotVectorU[knotCountU - 1 - 3];
    float ru = umax - umin;
    u = ru * u + umin;

    //float vmin = knotVectorV[4];
    float vmin = knotVectorV[3];
    //float vmax = knotVectorV[knotCountU - 1 - 4];
    float vmax = knotVectorV[knotCountV - 1 - 3];
    float rv = vmax - vmin;
    v = rv * v + vmin;

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < deboorPoints->rowCount(); i++){
        for(int j = 0; j < deboorPoints->columnCount(); j++){

            float bsU = bsplineDerivativeRecurive(u, DEGREE, i, knotVectorU);
            float bsV = bsplineDerivativeRecurive(v, DEGREE, j, knotVectorV);
            ifc::Point* point = (*deboorPoints)[i][j];
            const vec3& pos = point->getPosition();
            result += bsU * bsV * pos;
        }
    }
    return result;
}

bool SurfaceC2Cylind::replacePoint(ifc::Point *src, ifc::Point *dest) {
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

    int deboorRowCount = deboorPoints->rowCount();
    int deboorColumnCount = deboorPoints->columnCount();

    for(int i = 0; i < deboorRowCount;i++){
        for(int j = 0; j < deboorColumnCount; j++){
            ifc::Point* point = deboorPoints->getEntry(i,j);
            if(point == src){
                deboorPoints->setEntry(i, j, dest);
                return true;
            }
        }
    }

    return false;
}

void SurfaceC2Cylind::update(){
    const int DEGREE = 3;
    const int CUBIC_COUNT = 4;

    int deboorRowCount = deboorPoints->rowCount();
    int deboorColumnCount = deboorPoints->columnCount();

    int rowSegmentCount = deboorColumnCount - DEGREE;
    int columnSegmentCount = deboorRowCount - DEGREE;

    Matrix<vec4> bezierPoints(columnSegmentCount*CUBIC_COUNT,
                              rowSegmentCount*CUBIC_COUNT);

    for(int i = 0; i < deboorRowCount; i++){
        const std::vector<ifc::Point*>& row = (*deboorPoints)[i];
        int bezierIndex = 0;
        vector<vec4> bezierRow(bezierPoints.columnCount());

        for(int j = 0; j < rowSegmentCount; j++){
            const vec3& pos0 = row[j+0]->getPosition();
            const vec3& pos1 = row[j+1]->getPosition();
            const vec3& pos2 = row[j+2]->getPosition();
            const vec3& pos3 = row[j+3]->getPosition();

            bezierRow[bezierIndex++] =
                    vec4( (pos0 + pos1 * 4.0f + pos2) / 6.0f, 1);
            bezierRow[bezierIndex++] = vec4((pos1*4.0f + pos2*2.0f)/6.0f, 1);
            bezierRow[bezierIndex++] = vec4((pos1*2.0f + pos2*4.0f)/6.0f, 1);
            bezierRow[bezierIndex++] = vec4((pos1 + pos2*4.0f + pos3)/6.0f, 1);
        }
        bezierPoints[i] = bezierRow;
    }

    Matrix<vec4> bezierPointsFinal(columnSegmentCount*CUBIC_COUNT,
                                   rowSegmentCount*CUBIC_COUNT);
    for(int i = 0; i < bezierPoints.columnCount(); i++){
        const std::vector<vec4>& column = bezierPoints.getColumn(i);
        int bezierIndex = 0;
        vector<vec4> bezierColumn(bezierPoints.rowCount());

        for(int j = 0; j < columnSegmentCount; j++){
            const vec3& pos0 =
                    vec3(column[j+0].x, column[j+0].y, column[j+0].z);
            const vec3& pos1 =
                    vec3(column[j+1].x, column[j+1].y, column[j+1].z);
            const vec3& pos2 =
                    vec3(column[j+2].x, column[j+2].y, column[j+2].z);
            const vec3& pos3 =
                    vec3(column[j+3].x, column[j+3].y, column[j+3].z);

            bezierColumn[bezierIndex++] =
                    vec4( (pos0 + pos1 * 4.0f + pos2) / 6.0f, 1);
            bezierColumn[bezierIndex++] = vec4((pos1*4.0f + pos2*2.0f)/6.0f, 1);
            bezierColumn[bezierIndex++] = vec4((pos1*2.0f + pos2*4.0f)/6.0f, 1);
            bezierColumn[bezierIndex++] = vec4((pos1 + pos2*4.0f + pos3)/6.0f, 1);
        }
        bezierPointsFinal.setColumn(i, bezierColumn);
    }

    bezierPoints = bezierPointsFinal;

    // Create Bezier Patches
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            Matrix<vec4> patchPoints(CUBIC_COUNT, CUBIC_COUNT);

            for(int rowIndex = 0; rowIndex < CUBIC_COUNT; rowIndex++){
                const vector<vec4>& row =
                        bezierPoints[i*CUBIC_COUNT + rowIndex];
                vector<vec4> patchRow(CUBIC_COUNT);
                for(int colIndex = 0; colIndex < CUBIC_COUNT; colIndex++){
                    patchRow[colIndex] = row[j*CUBIC_COUNT + colIndex];
                }
                patchPoints[rowIndex] = patchRow;
            }

            BicubicBezierPatch* patch  = (patches)[i][j];
            if(patch != NULL){
                delete patch;
            }
            patch = new BicubicBezierPatch(patchPoints);
            (patches)[i][j] = patch;
        }
    };
}

void SurfaceC2Cylind::drawPolygon(const glm::mat4& VP, int segments) {
    Color color(0.6,0.6,0.6,1);
    setSurfaceColor(color);
    glLineWidth((GLfloat)lineWidth);


    for(int i = 0; i < deboorPoints->rowCount(); i++){
        std::vector<ifc::Point*> row = (*deboorPoints)[i];

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

    for(int i = 0; i < deboorPoints->columnCount(); i++){
        std::vector<ifc::Point*> column = deboorPoints->getColumn(i);

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