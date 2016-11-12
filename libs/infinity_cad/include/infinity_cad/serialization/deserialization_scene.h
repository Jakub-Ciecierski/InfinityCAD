#ifndef IFC_DESERIALIZATIONSCENE_H
#define IFC_DESERIALIZATIONSCENE_H

#include <infinity_cad/rendering/render_object.h>
#include "infinity_cad/rendering/scene/object_factory.h"

#include <string>
#include <vector>
#include <memory>

class DeserializationScene
{
private:
    std::vector<RenderObject*> points;
    std::vector<std::shared_ptr<SurfaceC2Cylind>> surfaces_;

    const std::string RECT_SURFACE_STR = "R";
    const std::string CYLIND_SURFACE_STR = "C";
    const std::string HORIZONTAL_CYLIND_SURFACE_STR = "H";
    const std::string VERTICAL_CYLIND_SURFACE_STR = "V";

    const int NO_SOLUTION = -1;

    void loadObject(std::string type, std::string name,
                    std::ifstream &istream);

    void PrintStatus();
public:
    DeserializationScene();
    std::vector<std::shared_ptr<SurfaceC2Cylind>> surfaces(){
        return surfaces_;
    };

    void load(std::string filepath);
};

#endif // IFC_DESERIALIZATIONSCENE_H
