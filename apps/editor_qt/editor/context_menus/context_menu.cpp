#include "context_menu.h"

ContextMenu::ContextMenu()
{

}

void ContextMenu::addAction(std::string name, Handler handler){
    QMenu::addAction(QString::fromStdString(name));

    handlerMap[name] = handler;
}

void ContextMenu::addAction(std::string name){
    QMenu::addAction(QString::fromStdString(name));
}

QAction* ContextMenu::show(const QPoint& pos) {
    QAction* selectedItem = exec(pos);

    return selectedItem;
}

void ContextMenu::handle(const QAction* a,
                         const QList<QTreeWidgetItem *>& selected){
    if(a == NULL) return;
    if(selected.empty()) return;

    QTreeWidgetItem* item = selected.at(0);

    std::string str = a->text().toStdString();
    std::string selectedObject = item->text(0).toStdString();

    // TODO handle proper non existing str
    Handler handler = handlerMap[str];

    handler.execute(selectedObject);
}
