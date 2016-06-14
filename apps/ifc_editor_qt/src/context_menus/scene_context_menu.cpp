#include <context_menus/scene_context_menu.h>


SceneContextMenu::SceneContextMenu()
{
    runForOneSelectedItem = false;
}

void SceneContextMenu::addAction(std::string name){
    QMenu::addAction(QString::fromStdString(name));
}

void SceneContextMenu::addHandler(SceneCMHandler handler){
    handlerMap[handler.getName()] = handler;
}

void SceneContextMenu::addHandlerAndAction(SceneCMHandler handler){
    QMenu::addAction(QString::fromStdString(handler.getName()));
    handlerMap[handler.getName()] = handler;
}

QAction* SceneContextMenu::show(const QPoint& pos) {
    QAction* selectedItem = exec(pos);

    return selectedItem;
}

void SceneContextMenu::handle(const QAction* a,
                              const std::vector<Item*>& selectedItems){
    if(a == NULL) return;
    if(selectedItems.empty()) return;
    std::string actionString = a->text().toStdString();

    for(unsigned int i = 0; i < selectedItems.size(); i++){
        Item* item = selectedItems[i];
        if(item == NULL || item->treeItem == NULL)  continue;

        SceneCMHandler handler = handlerMap[actionString];

        handler.execute(item);

        if(runForOneSelectedItem) return;
    }

}
