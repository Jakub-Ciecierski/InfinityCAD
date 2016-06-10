#include "deserialization_scene.h"
#include <fstream>
#include <iostream>
#include <string_utils.h>

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <system/object_manager.h>

using namespace std;

DeserializationScene::DeserializationScene(){

}

int DeserializationScene::getPointIndex(RenderObject* point){
    for(unsigned int i = 0; i < points.size(); i++){
        if(point == points[i])
            return i;
    }
    return NO_SOLUTION;
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
    ObjectManager& objectManager = ObjectManager::getInstance();

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

            Item* object = objectManager.addObject(RB_POINT_TYPE);
            object->object->moveTo(x,y,z);
            pointItems.push_back(object);
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
    }
    istream.close();
}

void DeserializationScene::loadObject(string type,
                                      string name, ifstream &istream){

    ObjectManager& objectManager = ObjectManager::getInstance();
    objectManager.getSceneTree()->deactivateAll();

    string line;

    if(type == OBJ_NAME_CURVE_BEZIER){
        Item* item = objectManager.addBezierCurve(name);

        std::getline(istream, line);
        int pointsNumber = atoi(line.c_str());

        std::getline(istream, line);
        vector<string> pointIDs = str_util::splitString(line, " ");

        for(unsigned int i = 0; i < pointIDs.size(); i++){
            if(pointIDs[i] == "") continue;
            int id = atoi(pointIDs[i].c_str());
            if(id < 0)
                continue;
            objectManager.addChildItem(item, pointItems[id]);
        }
    }

    if(type == OBJ_NAME_CURVE_BSPLINE){
        Item* item = objectManager.addObject(RB_BSPLINE_TYPE, name);

        std::getline(istream, line);
        int pointsNumber = atoi(line.c_str());

        std::getline(istream, line);
        vector<string> pointIDs = str_util::splitString(line, " ");

        for(unsigned int i = 0; i < pointIDs.size(); i++){
            if(pointIDs[i] == "") continue;
            int id = atoi(pointIDs[i].c_str());
            if(id < 0)
                continue;
            objectManager.addChildItem(item, pointItems[id]);
        }
    }

    if(type == OBJ_NAME_CURVE_INTERP){
        Item* item = objectManager.addObject(RB_BSPLINE_INTERPOLATING_TYPE, name);

        std::getline(istream, line);
        int pointsNumber = atoi(line.c_str());

        std::getline(istream, line);
        vector<string> pointIDs = str_util::splitString(line, " ");

        for(unsigned int i = 0; i < pointIDs.size(); i++){
            if(pointIDs[i] == "") continue;
            int id = atoi(pointIDs[i].c_str());
            objectManager.addChildItem(item, pointItems[id]);
        }
    }
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
/*
        Matrix<ifc::Point*> pointsMatrix(n,m);
        vector<Item*> items;
        int pointCount = n*m;

        std::getline(istream, line);
        vector<string> pointIDs = str_util::splitString(line, " ");

        int index = 0;
        for(int i = 0; i < n; i++){
            vector<ifc::Point*> row;
            for(int j = 0; j < m; j++){

                int id = atoi(pointIDs[index].c_str());
                Item* item = pointItems[id];
                items.push_back(item);

                row.push_back(static_cast<ifc::Point*>(item->object));

                index++;
            }
            pointsMatrix[i] = row;
        }
*/
        Matrix<ifc::Point*> pointsMatrix(m,n);
        vector<Item*> items;
        int pointCount = n*m;

        std::getline(istream, line);
        vector<string> pointIDs = str_util::splitString(line, " ");

        int index = 0;
        for(int i = 0; i < m; i++){
            vector<ifc::Point*> row;
            for(int j = 0; j < n; j++){

                int id = atoi(pointIDs[index].c_str());
                Item* item = pointItems[id];
                items.push_back(item);

                row.push_back(static_cast<ifc::Point*>(item->object));

                index++;
            }
            pointsMatrix[i] = row;
        }
        if(surfaceType == RECT_SURFACE_STR){
            objectManager.addSurfaceC2Rect(name, pointsMatrix, items);
        }else if(surfaceType == CYLIND_SURFACE_STR){
            objectManager.addSurfaceC2Cylind(name, pointsMatrix, items);
        }
    }
    if(type == OBJ_NAME_SURFACE_BEZIER){
        std::getline(istream, line);
        vector<string> data = str_util::splitString(line, " ");

        int n = atoi(data[0].c_str());
        int m = atoi(data[1].c_str());
        string surfaceType = data[2];
        string cylinderMode;
        if(surfaceType == CYLIND_SURFACE_STR){
            cylinderMode = data[3];
        }

        Matrix<ifc::Point*> pointsMatrix(n,m);
        vector<Item*> items;
        int pointCount = n*m;

        std::getline(istream, line);
        vector<string> pointIDs = str_util::splitString(line, " ");

        int index = 0;
        for(int i = 0; i < n; i++){
            vector<ifc::Point*> row;
            for(int j = 0; j < m; j++){

                int id = atoi(pointIDs[index].c_str());
                Item* item = pointItems[id];
                items.push_back(item);

                row.push_back(static_cast<ifc::Point*>(item->object));

                index++;
            }
            pointsMatrix[i] = row;
        }

        if(surfaceType == RECT_SURFACE_STR){
            objectManager.addSurfaceC0Rect(name, pointsMatrix, items);
        }else if(surfaceType == CYLIND_SURFACE_STR){
            ;
            // TODO CYLIND C0
            //objectManager.addSurfaceC0Cylind(name, pointsMatrix, items);
        }
    }
}
