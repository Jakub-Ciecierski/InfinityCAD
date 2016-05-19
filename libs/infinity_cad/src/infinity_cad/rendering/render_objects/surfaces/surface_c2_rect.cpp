//
// Created by jakub on 5/15/16.
//

#include <infinity_cad/rendering/render_objects/surfaces/surface_c2_rect.h>
#include <infinity_cad/rendering/scene/object_factory.h>

using namespace glm;
using namespace std;

//-----------------------//
//  CONSTRUCTORS
//-----------------------//

SurfaceC2Rect::SurfaceC2Rect(SceneID id, std::string name,
                             int n, int m,
                             float width, float height) :
        Surface(id, name, n, m), width(width), height(height){
    build();
    update();

    objectType = OBJ_TYPE_SURFACE_BSPLINE_RECT;
}

SurfaceC2Rect::SurfaceC2Rect(SceneID id, std::string name,
                             Matrix<ifc::Point*> points) :
        Surface(id, name, 0, 0){

    n = points.rowCount() - 3;
    m = points.columnCount() - 3;

    patches = Matrix<BicubicBezierPatch*>(n, m, NULL);
    deboorPoints = new Matrix<ifc::Point*>(points);

    for(int i = 0; i < points.rowCount(); i++){
        for(int j = 0; j < points.columnCount(); j++){
            allPoints.push_back((*deboorPoints)[i][j]);
            components.push_back((*deboorPoints)[i][j]);
        }
    }

    update();

    objectType = OBJ_TYPE_SURFACE_BSPLINE_RECT;
}

SurfaceC2Rect::~SurfaceC2Rect(){
    delete deboorPoints;
}

//-----------------------//
//  PROTECTED
//-----------------------//

void SurfaceC2Rect::build(){
    vec3 origin(0,0,0);

    int rowCount = n + 3;
    int columnCount = m + 3;
    deboorPoints = new Matrix<ifc::Point*>(rowCount, columnCount);

    float x,y,z;
    x = y = z = 0;
    float dx = width / (float)rowCount;
    float dy = height / (float)columnCount;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    for(int i = 0; i < rowCount; i++){
        x = 0;
        for(int j = 0; j < columnCount; j++){
            (*deboorPoints)[i][j]
                    = objectFactory.createPoint(createPointName(n,m,j,i));

            allPoints.push_back((*deboorPoints)[i][j]);
            components.push_back((*deboorPoints)[i][j]);

            (*deboorPoints)[i][j]->moveTo(origin);
            (*deboorPoints)[i][j]->move(x, y, z);
            x += dx;
        }
        y += dy;
    }

    // for serialization
    allPointsMatrix = *deboorPoints;
}

//-----------------------//
//  PUBLIC
//-----------------------//

void SurfaceC2Rect::update(){
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

void SurfaceC2Rect::drawPolygon(const glm::mat4& VP, int segments) {
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