#ifndef DESERIALIZATIONSCENE_H
#define DESERIALIZATIONSCENE_H

#include <infinity_cad/rendering/scene/object_factory.h>
#include <widgets/scene_list/entities/item.h>
#include <string>

class DeserializationScene
{
private:
    std::vector<RenderObject*> points;
    std::vector<Item*> pointItems;

    const std::string RECT_SURFACE_STR = "R";
    const std::string CYLIND_SURFACE_STR = "C";
    const std::string HORIZONTAL_CYLIND_SURFACE_STR = "H";
    const std::string VERTICAL_CYLIND_SURFACE_STR = "V";

    const int NO_SOLUTION = -1;

    int getPointIndex(RenderObject* point);

    void loadObject(std::string type,  std::string name,
                    std::ifstream &istream);
public:
    DeserializationScene();

    void load(std::string filepath);
};

#endif // DESERIALIZATIONSCENE_H
