#ifndef SERIALIZATIONSCENE_H
#define SERIALIZATIONSCENE_H

#include <infinity_cad/rendering/scene/scene.h>
#include <infinity_cad/object_types.h>
#include <sstream>

class SerializationScene
{
private:
    Scene* scene;

    std::vector<RenderObject*> points;

    const int NO_SOLUTION = -1;

    const std::string END_DATA_STR = "END";
    const std::string RECT_SURFACE_STR = "R";
    const std::string CYLIND_SURFACE_STR = "C";
    const std::string HORIZONTAL_CYLIND_SURFACE_STR = "H";
    const std::string VERTICAL_CYLIND_SURFACE_STR = "V";

    int getPointIndex(RenderObject* point);

    void savePoints(std::stringstream& ss);
    void saveObjects(std::stringstream& ss);
    void saveCurves(std::stringstream& ss, const ObjectType& type,
                    const std::vector<RenderObject*>& curves);
    void saveSurfaces(std::stringstream& ss, const ObjectType& type,
                      const std::vector<RenderObject*>& surfaces);

public:
    SerializationScene(Scene* scene);

    void save(std::string filepath);


};

#endif // SERIALIZATIONSCENE_H
