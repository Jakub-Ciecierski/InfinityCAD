#include "scene_context_menu.h"


SceneContextMenu::SceneContextMenu()
{

}

void SceneContextMenu::addAction(std::string name){
    QMenu::addAction(QString::fromStdString(name));
}

void SceneContextMenu::addActionAndHandler(std::string name,
                                           SceneCMHandler handler){
    QMenu::addAction(QString::fromStdString(name));

    handlerMap[name] = handler;
}

void SceneContextMenu::addHandler(std::string name,
                                  SceneCMHandler handler){
    handlerMap[name] = handler;
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

        // TODO handle proper non existing str
        SceneCMHandler handler = handlerMap[actionString];

        handler.execute(item);
    }

}
