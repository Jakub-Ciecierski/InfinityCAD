//
// Created by jakub on 5/2/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface_c0_rect.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/settings/settings.h>
#include <infinity_cad/math/math.h>
#include <ifc_gpu/surfaces/bezier_surface_gpu.h>
#include <algorithm>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

SurfaceC0Rect::SurfaceC0Rect(SceneID id, std::string name,
                             int n, int m, float width, float height) :
        Surface(id, name, n, m), width(width), height(height){
    this->build();

    objectType = OBJ_TYPE_SURFACE_BEZIER_RECT;
}

SurfaceC0Rect::SurfaceC0Rect(SceneID id, std::string name,
                             Matrix<ifc::Point*> points) :
        Surface(id, name, 0, 0){
    buildPatchesFromMatrix(points);

    objectType = OBJ_TYPE_SURFACE_BEZIER_RECT;
}

SurfaceC0Rect::~SurfaceC0Rect(){
    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            delete patches[i][j];
        }
    }
}

//-----------------------//
//  PRIVATE
//-----------------------//

void SurfaceC0Rect::buildPatchesFromMatrix(const Matrix<ifc::Point*>& points){

    n = (points.rowCount() + 1) / (CUBIC_COUNT - 1);
    m = (points.columnCount() + 1) / (CUBIC_COUNT - 1);

    patches = Matrix<BicubicBezierPatch*>(n, m, NULL);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            Matrix<ifc::Point*> patchPoints(CUBIC_COUNT, CUBIC_COUNT);
            for(int k = 0; k < CUBIC_COUNT; k++){
                vector<ifc::Point*> row = points[i*CUBIC_COUNT-i + k];
                for(int l = 0; l < CUBIC_COUNT; l++){
                    patchPoints[k][l] = row[j * CUBIC_COUNT-j + l];

                    allPoints.push_back(patchPoints[k][l]);
                    components.push_back(patchPoints[k][l]);


                    if(k == 0 && l == CUBIC_COUNT - 1) {
                        points[l][k]->point_surface_type
                                (ifc::PointSurfaceType::U0V1);
                    }
                    if(k == CUBIC_COUNT - 1 && l == 0) {
                        points[l][k]->point_surface_type
                                (ifc::PointSurfaceType::U1V0);
                    }
                    if(k == CUBIC_COUNT - 1 && l == CUBIC_COUNT - 1) {
                        points[l][k]->point_surface_type
                                (ifc::PointSurfaceType::U1V1);
                    }
                    if(k == 0 && l == 0) {
                        points[l][k]->point_surface_type
                                (ifc::PointSurfaceType::U0V0);
                    }

                }
            }
            BicubicBezierPatch* patch = new BicubicBezierPatch(patchPoints);
            patches[i][j] = patch;
        }
    }

    allPointsMatrix = points;
}

Matrix<ifc::Point*> SurfaceC0Rect::getMatrixFormOfAllPatches(){
    int numberOfPointsInRow = CUBIC_COUNT*n - (n-1);
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

BicubicBezierPatch*SurfaceC0Rect::createPatch(int n, int m,
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

            if(i == 0 && j == CUBIC_COUNT - 1)
                points[j][i]->point_surface_type(ifc::PointSurfaceType::U0V1);
            if(i == CUBIC_COUNT - 1 && j == 0)
                points[j][i]->point_surface_type(ifc::PointSurfaceType::U1V0);
            if(i == CUBIC_COUNT - 1 && j == CUBIC_COUNT - 1)
                points[j][i]->point_surface_type(ifc::PointSurfaceType::U1V1);
            if(i == 0 && j == 0)
                points[j][i]->point_surface_type(ifc::PointSurfaceType::U0V0);
        }
        x += dX;
    }
    BicubicBezierPatch* patch = new BicubicBezierPatch(points);

    return patch;
}

Matrix<ifc::Point*> SurfaceC0Rect::initC0Points(int n, int m){
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

//-----------------------//
//  PROTECTED
//-----------------------//

void SurfaceC0Rect::build() {
    vec3 startPos(0,0,0);

    float patchWidth = width/(float)n;
    float patchHeight = height/(float)m;

    for(int i = 0; i < n;i++){
        for(int j = 0; j < m; j++){
            patches[i][j] = createPatch(i,j, patchWidth, patchHeight, startPos);
        }
    }
    allPointsMatrix = getMatrixFormOfAllPatches();
}
