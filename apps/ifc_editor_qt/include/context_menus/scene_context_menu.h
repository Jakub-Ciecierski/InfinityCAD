#ifndef SCENECONTEXTMENU_H
#define SCENECONTEXTMENU_H

#include <QMenu>
#include <QPoint>
#include <context_menus/scene_cm_handler.h>
#include <QTreeWidgetItem>
#include <context_menus/context_menu.h>
#include <widgets/scene_list/entities/item.h>

class SceneContextMenu : public ContextMenu
{
private:

    std::map<std::string, SceneCMHandler> handlerMap;

public:
    SceneContextMenu();

    void addAction(std::string name);
    void addActionAndHandler(std::string name, SceneCMHandler handler);
    void addHandler(std::string name, SceneCMHandler handler);
    virtual QAction* show(const QPoint& pos);

    void handle(const QAction* a,
                const std::vector<Item*>& selectedItems);

};
#endif // SCENECONTEXTMENU_H
