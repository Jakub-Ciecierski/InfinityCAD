//
// Created by jakub on 5/2/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface_c0_rect.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/settings/settings.h>
#include <infinity_cad/math/math.h>
#include <ifc_gpu/surfaces/bezier_surface_gpu.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

SurfaceC0Rect::SurfaceC0Rect(SceneID id, std::string name,
                             int n, int m, float width, float height) :
        Surface(id, name, n, m), width(width), height(height){
    this->build();
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
}
