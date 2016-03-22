#include "objects_tree.h"
#include <QMenu>
#include <editor_window.h>

ObjectsTree::ObjectsTree(QWidget* parent) :
    QTreeWidget(parent)
{
    setupContextMenu();
}

void ObjectsTree::setupContextMenu(){
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));
}

void ObjectsTree::ShowContextMenu(const QPoint& pos){
    QPoint globalPos = this->mapToGlobal(pos);

    ContextMenu& menu = EditorWindow::getInstance().
            getObjectsListContextMenu();
    QAction* selectedItem = menu.show(globalPos);

    menu.handle(selectedItem, this->selectedItems());
}

#include "moc_objects_tree.cpp"
