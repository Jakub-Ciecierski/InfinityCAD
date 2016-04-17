#include <iostream>
#include <context_menus/context_menu.h>

ContextMenu::ContextMenu()
{

}

void ContextMenu::addAction(std::string name){
    QMenu::addAction(QString::fromStdString(name));
}

void ContextMenu::addActionAndHandler(std::string name, Handler handler){
    QMenu::addAction(QString::fromStdString(name));

    handlerMap[name] = handler;
}

void ContextMenu::addHandler(std::string name, Handler handler){
    handlerMap[name] = handler;
}

QAction* ContextMenu::show(const QPoint& pos) {
    QAction* selectedItem = exec(pos);

    return selectedItem;
}

void ContextMenu::handle(const QAction* a,
                         const QList<QTreeWidgetItem *>& selected){
    if(a == NULL) return;
    if(selected.empty()) return;
    std::string actionString = a->text().toStdString();

    for(int i = 0; i < selected.size(); i++){
        QTreeWidgetItem* item = selected.at(i);

        std::string selectedObject = item->text(0).toStdString();

        // TODO handle proper non existing str
        Handler handler = handlerMap[actionString];

        handler.execute(selectedObject);
    }

}
