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

protected:
    bool runForOneSelectedItem;

public:
    SceneContextMenu();

    void addHandlerAndAction(SceneCMHandler handler);
    void addAction(std::string name);
    void addHandler(SceneCMHandler handler);

    virtual QAction* show(const QPoint& pos);

    void handle(const QAction* a,
                const std::vector<Item*>& selectedItems);

};
#endif // SCENECONTEXTMENU_H
