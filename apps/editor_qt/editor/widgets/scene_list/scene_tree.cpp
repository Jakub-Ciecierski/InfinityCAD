#include "scene_tree.h"

#include <QMenu>
#include <editor_window.h>
#include <iostream>
#include "system/object_manager.h"

using namespace std;

SceneTree::SceneTree(QWidget* parent) :
    QTreeWidget(parent)
{
    setupContextMenu();

    this->connect(this,
                 SIGNAL(itemPressed(QTreeWidgetItem*,int)),
                 this,
                 SLOT(myitemActivated(QTreeWidgetItem*,int)));
    this->connect(this,
                 SIGNAL(itemSelectionChanged()),
                 this,
                 SLOT(myitemSelectionChanged()));

}

SceneID SceneTree::getID(std::string name){
    int i = getItemIndex(name);
    if(i < 0) throw std::invalid_argument(name + " - No such Object");

    Item item = items[i];
    return item.id;
}

int SceneTree::getItemIndex(std::string name){
    for(unsigned int i = 0; i < items.size(); i++){
        Item item = items[i];
        if (name == item.name)
            return i;
    }
    return -1;
}

int SceneTree::getItemIndex(QTreeWidgetItem* treeItem){
    for(unsigned int i = 0; i < items.size(); i++){
        Item item = items[i];
        if (treeItem == item.treeItem)
            return i;
    }
    return -1;
}

void SceneTree::addObject(string name, SceneID id){
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(this);
    treeItem->setText(0, QString::fromStdString(name));

    this->addTopLevelItem(treeItem);

    Item item(id, treeItem, name);

    this->items.push_back(item);
}

SceneID SceneTree::deleteObject(string name){
    int i = getItemIndex(name);
    if(i < 0) throw std::invalid_argument(name + " - No such Object");

    Item item = this->items[i];

    delete item.treeItem;
    items.erase(items.begin() + i);

    return item.id;
}

void SceneTree::changeName(string srcName, string dstName){
    int i = getItemIndex(srcName);
    if(i < 0) throw std::invalid_argument(srcName + " - No such Object");

    Item& item = this->items[i];
    item.name = dstName;
    item.treeItem->setText(0, QString::fromStdString(dstName));
}

void SceneTree::setupContextMenu(){
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));
}

void SceneTree::ShowContextMenu(const QPoint& pos){
    QPoint globalPos = this->mapToGlobal(pos);

    ContextMenu& menu = EditorWindow::getInstance().
            getSceneListContextMenu();

    QAction* selectedItem = menu.show(globalPos);

    menu.handle(selectedItem, this->selectedItems());
}

//-----------------------------------------------------------//
//  EVENTS
//-----------------------------------------------------------//


void SceneTree::myitemActivated(QTreeWidgetItem* treeItem, int column){
    int i = getItemIndex(treeItem);
    if(i < 0) return;

    Item item = items[i];
    ObjectManager::getInstance().setActive(item.id);
}

void SceneTree::myitemSelectionChanged(){
    for(int i = 0;i < items.size(); i++){
        Item item = items[i];
        ObjectManager::getInstance().setDeactive(item.id);
    }
}

#include "moc_scene_tree.cpp"
