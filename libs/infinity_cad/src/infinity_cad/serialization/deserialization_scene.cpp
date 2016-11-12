#include "infinity_cad/serialization/deserialization_scene.h"

#include <fstream>
#include <iostream>
#include <string_utils.h>

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

DeserializationScene::DeserializationScene(){
}

bool isValidType(std::string line){
    return (line == OBJ_NAME_CURVE_BEZIER ||
            line == OBJ_NAME_CURVE_BSPLINE ||
            line == OBJ_NAME_CURVE_INTERP ||
            line == OBJ_NAME_SURFACE_BEZIER ||
            line == OBJ_NAME_SURFACE_BSPLINE);
}

void DeserializationScene::load(std::string filepath){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    std::cout << "Loading Model..." << std::endl;

    std::ifstream istream(filepath);
    if(istream){
        std::string line;

        int numberOfPoints = -1;
        while(numberOfPoints < 1 && !istream.eof()){
            std::getline(istream, line);
            numberOfPoints = stoi(line);
        }
        // Points
        for(int i = 0; i < numberOfPoints; i++){
            std::getline(istream, line);
            // Change commas for dots !
            line = str_util::replaceChar(line, ',', '.');

            vector<string> positionStr = str_util::splitString(line, " ");
            if(positionStr.size() < 3){
                throw new std::invalid_argument("Invalid Point format");
            }

            float x = atof(positionStr[0].c_str());
            float y = atof(positionStr[1].c_str());
            float z = atof(positionStr[2].c_str());

            RenderObject* p = objectFactory.createPoint("point");
            p->moveTo(x,y,z);
            points.push_back(p);
        }

        int numberOfObject = -1;
        while(numberOfObject < 1 && !istream.eof()){
            std::getline(istream, line);
            numberOfObject = stoi(line);
        }

        for(int n = 0; n < numberOfObject; n++){
            vector<string> typeName;
            do{
                std::getline(istream, line);
                typeName = str_util::splitString(line, " ");
            } while(!isValidType(typeName[0]) && !istream.eof());

            string name = "";
            for(int p = 1; p < typeName.size();p++){
                name += typeName[p];
                if(p != typeName.size() - 1) name += " ";
            }

            loadObject(typeName[0], name, istream);
        }
    }else{
        std::cout << "No Model file found" << std::endl;
    }
    istream.close();
    PrintStatus();
}

void DeserializationScene::loadObject(string type,
                                      string name, ifstream &istream){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    string line;
    if(type == OBJ_NAME_SURFACE_BSPLINE){
        std::getline(istream, line);
        vector<string> data = str_util::splitString(line, " ");

        int n = atoi(data[0].c_str());
        int m = atoi(data[1].c_str());
        string surfaceType = data[2];
        string cylinderMode;
        if(surfaceType == CYLIND_SURFACE_STR){
            cylinderMode = data[3];
        }

        Matrix<ifc::Point*> pointsMatrix(m,n);
        int pointCount = n*m;

        std::getline(istream, line);
        vector<string> pointIDs = str_util::splitString(line, " ");

        int index = 0;
        for(int i = 0; i < m; i++){
            vector<ifc::Point*> row;
            for(int j = 0; j < n; j++){

                int id = atoi(pointIDs[index].c_str());
                RenderObject* point = points[id];

                row.push_back(static_cast<ifc::Point*>(point));

                index++;
            }
            pointsMatrix[i] = row;
        }
        if(surfaceType == RECT_SURFACE_STR){
            SurfaceC2Rect* surface
                    = objectFactory.createSurfaceC2Rect(name, pointsMatrix);
        }else if(surfaceType == CYLIND_SURFACE_STR){
            SurfaceC2Cylind* surface
                    = objectFactory.createSurfaceC2Cylind(name, pointsMatrix);
            std::shared_ptr<SurfaceC2Cylind> surface_ptr
                    = std::shared_ptr<SurfaceC2Cylind>(surface);
            surfaces_.push_back(surface_ptr);
        }
    }
}

void DeserializationScene::PrintStatus(){
    std::cout
    << "C2 Cylinder Surface count: " << surfaces_.size()
    << std::endl;

    for(unsigned int i = 0;i < surfaces_.size(); i++){

        Matrix<BicubicBezierPatch*>& patches
                = surfaces_[i]->GetBicubicBezierPatches();
        int patchesCount = patches.columnCount() * patches.rowCount();
        std::cout
        << "Surface[" << i <<  "] patch count: "
        << patchesCount
        << std::endl;
    }
}