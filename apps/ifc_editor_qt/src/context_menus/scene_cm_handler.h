#ifndef SCENECMHANDLER_H
#define SCENECMHANDLER_H

#include <functional>
#include <widgets/scene_list/entities/item.h>

class SceneCMHandler
{
public:
    SceneCMHandler();

    SceneCMHandler(std::function<void(Item*)> handler);

    SceneCMHandler(std::string name,
                   std::function<void(Item*)> handler);

    std::string getName();

    void execute(Item* item);

private:
    std::string name;

    std::function<void(Item*)> handler;
};
#endif // SCENECMHANDLER_H
