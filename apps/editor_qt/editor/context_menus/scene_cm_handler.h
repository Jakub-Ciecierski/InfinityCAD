#ifndef SCENECMHANDLER_H
#define SCENECMHANDLER_H

#include <functional>
#include <widgets/scene_list/entities/item.h>

class SceneCMHandler
{
public:
    SceneCMHandler();

    SceneCMHandler(std::function<void(Item*)> handler);

    void execute(Item* item);

private:
    std::function<void(Item*)> handler;
};
#endif // SCENECMHANDLER_H
