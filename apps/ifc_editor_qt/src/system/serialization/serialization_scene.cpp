#include "serialization_scene.h"
#include <infinity_cad/rendering/render_objects/curves/spline.h>
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>
#include <fstream>

using namespace std;
using namespace glm;

SerializationScene::SerializationScene(Scene* scene) {
    this->scene = scene;
}

int SerializationScene::getPointIndex(RenderObject* point){
    for(unsigned int i = 0; i < points.size(); i++){
        if(point == points[i])
            return i;
    }
    return NO_SOLUTION;
}

void SerializationScene::savePoints(stringstream& ss){
    points = scene->getObjects(OBJ_TYPE_POINT);
    ss << to_string(points.size()) << std::endl;

    for(unsigned int i = 0; i < points.size(); i++){
        const vec3& pos = points[i]->getPosition();
        float x = pos.x;
        float y = pos.y;
        float z = pos.z;

        ss << x << " " << y << " " << z << std::endl;
    }
}

void SerializationScene::saveObjects(stringstream& ss){
    vector<RenderObject*> bezierCurves = scene->getObjects(OBJ_TYPE_CURVE_BEZIER);
    vector<RenderObject*> bsplineCurves = scene->getObjects(OBJ_TYPE_CURVE_BSPLINE);
    vector<RenderObject*> interpCurves = scene->getObjects(OBJ_TYPE_CURVE_INTERP);

    vector<RenderObject*> bezierSurfaceRect = scene->getObjects(OBJ_TYPE_SURFACE_BEZIER_RECT);
    vector<RenderObject*> bezierSurfaceCylind = scene->getObjects(OBJ_TYPE_SURFACE_BEZIER_CYLIND);
    vector<RenderObject*> bsplineSurfaceRect = scene->getObjects(OBJ_TYPE_SURFACE_BSPLINE_RECT);
    vector<RenderObject*> bsplineSurfaceCylind = scene->getObjects(OBJ_TYPE_SURFACE_BSPLINE_CYLIND);

    int objectsCount = bezierCurves.size() + bsplineCurves.size() + interpCurves.size()
            + bezierSurfaceRect.size() + bezierSurfaceCylind.size() +
            bsplineSurfaceRect.size() + bsplineSurfaceCylind.size();

    ss << objectsCount << std::endl;

    saveCurves(ss, OBJ_TYPE_CURVE_BEZIER, bezierCurves);
    saveCurves(ss, OBJ_TYPE_CURVE_BSPLINE, bsplineCurves);
    saveCurves(ss, OBJ_TYPE_CURVE_INTERP, interpCurves);

    saveSurfaces(ss, OBJ_TYPE_SURFACE_BEZIER_RECT, bezierSurfaceRect);
    saveSurfaces(ss, OBJ_TYPE_SURFACE_BEZIER_CYLIND, bezierSurfaceCylind);
    saveSurfaces(ss, OBJ_TYPE_SURFACE_BSPLINE_RECT, bsplineSurfaceRect);
    saveSurfaces(ss, OBJ_TYPE_SURFACE_BSPLINE_CYLIND, bsplineSurfaceCylind);
}

void SerializationScene::saveCurves(stringstream& ss, const ObjectType& type,
                                   const vector<RenderObject*>& curves){
    for(unsigned int i = 0; i < curves.size(); i++){
        Spline* spline = static_cast<Spline*>(curves[i]);

        ss << type.toString() << " " << spline->getName() << std::endl;

        vector<ifc::Point*> points = spline->getControlPoints();
        ss << points.size() << std::endl;

        for(unsigned int i = 0; i < points.size(); i++){
            int id = this->getPointIndex(points[i]);
            if(id == NO_SOLUTION){
                throw new std::invalid_argument("Wrong point serialization");
            }
            ss << id << " ";
        }
        ss << std::endl;
        ss << END_DATA_STR << std::endl;
    }
}


void SerializationScene::saveSurfaces(stringstream& ss, const ObjectType& type,
                                      const vector<RenderObject*>& surfaces){
    for(unsigned int i = 0; i < surfaces.size(); i++){
        Surface* surface = static_cast<Surface*>(surfaces[i]);
        MatrixMajor matrixMajor = surface->matrixMajor;

        const Matrix<ifc::Point*>& points = surface->getMatrixPoints();

        ss << type.toString() << " " << surface->getName() << std::endl;
        if(matrixMajor == MatrixMajor::ROW){
            int n = points.rowCount();
            int m = points.columnCount();
            ss << n << " " << m << " ";
            if(type == OBJ_TYPE_SURFACE_BEZIER_RECT ||
                    type == OBJ_TYPE_SURFACE_BSPLINE_RECT){
                ss << RECT_SURFACE_STR;
            }else if(type == OBJ_TYPE_SURFACE_BEZIER_CYLIND||
                     type == OBJ_TYPE_SURFACE_BSPLINE_CYLIND){
                // Vertical cylinder mode is hardcoded !!!
                ss << CYLIND_SURFACE_STR << " " << HORIZONTAL_CYLIND_SURFACE_STR;
            }
            ss << std::endl;

            for(int rowIndex = 0; rowIndex < m; rowIndex++){
                const std::vector<ifc::Point*>& row = points.getColumn(rowIndex);
                for(int columnIndex = 0; columnIndex < row.size(); columnIndex++){
                    int id = this->getPointIndex(points[columnIndex][rowIndex]);
                    if(id == NO_SOLUTION){
                        throw new std::invalid_argument("Wrong point serialization");
                    }
                    ss << id << " ";
                }
            }
        }else{
            int n = points.rowCount();
            int m = points.columnCount();
            ss << m << " " << n << " ";
            if(type == OBJ_TYPE_SURFACE_BEZIER_RECT ||
                    type == OBJ_TYPE_SURFACE_BSPLINE_RECT){
                ss << RECT_SURFACE_STR;
            }else if(type == OBJ_TYPE_SURFACE_BEZIER_CYLIND||
                     type == OBJ_TYPE_SURFACE_BSPLINE_CYLIND){
                // Vertical cylinder mode is hardcoded !!!
                ss << CYLIND_SURFACE_STR << " " << HORIZONTAL_CYLIND_SURFACE_STR;
            }
            ss << std::endl;

            for(int rowIndex = 0; rowIndex < n; rowIndex++){
                const std::vector<ifc::Point*>& row = points[rowIndex];
                for(int columnIndex = 0; columnIndex < row.size(); columnIndex++){
                    int id = this->getPointIndex(points[rowIndex][columnIndex]);
                    if(id == NO_SOLUTION){
                        throw new std::invalid_argument("Wrong point serialization");
                    }
                    ss << id << " ";
                }
            }
        }
        ss << std::endl;
        ss << END_DATA_STR << std::endl;
    }
}

/*

void SerializationScene::saveSurfaces(stringstream& ss, const ObjectType& type,
                                      const vector<RenderObject*>& surfaces){
    for(unsigned int i = 0; i < surfaces.size(); i++){
        Surface* surface = static_cast<Surface*>(surfaces[i]);
        const Matrix<ifc::Point*>& points = surface->getMatrixPoints();

        ss << type.toString() << " " << surface->getName() << std::endl;

        int n = points.rowCount();
        int m = points.columnCount();
        ss << n << " " << m << " ";
        if(type == OBJ_TYPE_SURFACE_BEZIER_RECT ||
                type == OBJ_TYPE_SURFACE_BSPLINE_RECT){
            ss << RECT_SURFACE_STR;
        }else if(type == OBJ_TYPE_SURFACE_BEZIER_CYLIND||
                 type == OBJ_TYPE_SURFACE_BSPLINE_CYLIND){
            // Vertical cylinder mode is hardcoded !!!
            ss << CYLIND_SURFACE_STR << " " << VERTICAL_CYLIND_SURFACE_STR;
        }
        ss << std::endl;

        for(int rowIndex = 0; rowIndex < n; rowIndex++){
            const std::vector<ifc::Point*>& row = points[rowIndex];
            for(int columnIndex = 0; columnIndex < row.size(); columnIndex++){
                int id = this->getPointIndex(points[rowIndex][columnIndex]);
                if(id == NO_SOLUTION){
                    throw new std::invalid_argument("Wrong point serialization");
                }
                ss << id << " ";
            }
        }
        ss << std::endl;
        ss << END_DATA_STR << std::endl;
    }
}
*/

void SerializationScene::save(std::string filepath){
    stringstream ss;

    savePoints(ss);
    saveObjects(ss);

    std::ofstream file;

    const string EXTENSION = ".mg";
    filepath += EXTENSION;

    file.open(filepath);
    file << ss.str();
    file.close();
}
