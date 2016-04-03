#include "scene_tree.h"

#include <QMenu>
#include <editor_window.h>
#include <iostream>
#include "system/object_manager.h"
#include "system/ic_names.h"
#include <widgets/scene_list/context_menus/scene_cmenu_factory.h>


using namespace std;

SceneTree::SceneTree(QWidget* parent) :
    QTreeWidget(parent)
{
    setupContextMenu();
    setupBinding();
    setupRootItems();
}

//-----------------------------//
//  PRIVATE
//-----------------------------//

void SceneTree::dropEvent(QDropEvent * event){
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        event->setDropAction(Qt::IgnoreAction);
        return;
    }
    QTreeWidgetItem* destinationTreeItem = itemFromIndex(index);
    ItemTMP* destItem = getItemByTree(destinationTreeItem);

    // Points to Bezier
    if(destItem != NULL && destItem->type == RB_BEZIER_NAME) {
        QList<QTreeWidgetItem *> selectedPoints =
                filterSelectedItems(RB_POINT_NAME);
        for(auto* selectedPoint : selectedPoints){

            destItem->treeItem->addChild(selectedPoint);

        }
    }

    //std::cout << "droped" << std::endl;
}

void SceneTree::setupBinding(){
    this->connect(this, SIGNAL(itemPressed(QTreeWidgetItem*,int)),
                 this, SLOT(myitemActivated(QTreeWidgetItem*,int)));
    this->connect(this, SIGNAL(itemSelectionChanged()),
                 this, SLOT(myitemSelectionChanged()));
}

void SceneTree::setupRootItems(){
    rootItems.resize(3);

    rootItems[TORUS_ROOT_INDEX] = RootTreeItem(RB_TORUS_NAME, "Toruses");
    rootItems[POINT_ROOT_INDEX] = RootTreeItem(RB_POINT_NAME, "Points");
    rootItems[BEZIER_ROOT_INDEX] = RootTreeItem(RB_BEZIER_NAME, "Bezier Curves");
}

void SceneTree::setupContextMenu(){
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    this->setAcceptDrops(true);
    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::InternalMove);
}


ContextMenu* SceneTree::getMenuBasedOnItems(
        QList<QTreeWidgetItem *>& selectedItems){
    SceneCMenuFactory& factory = SceneCMenuFactory::getInstance();

    int pointCount = 0;
    RootTreeItem& rootItem = rootItems[POINT_ROOT_INDEX];

    for(auto* item : selectedItems){
        int index = rootItem.item->indexOfChild(item);
        if(index >= 0) pointCount++;
    }
    // All selected are points
    if(pointCount == selectedItems.size()){
        return factory.getPointMenu(&rootItems[BEZIER_ROOT_INDEX]);
    }else{
        return factory.getDefaultMenu();
    }
}

QList<QTreeWidgetItem *> SceneTree::filterSelectedItems(string type){
    QList<QTreeWidgetItem *> filteredItems;

    QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
    for(unsigned int i = 0; i < selectedItems.size(); i++){
        ItemTMP* item  = getItemByTree(selectedItems[i]);
        if(item != NULL && item->type == type)
            filteredItems.push_back(selectedItems[i]);
    }

    return filteredItems;
}


QList<QTreeWidgetItem *> SceneTree::filterSelectedItems(){
    QList<QTreeWidgetItem *> filteredItems;

    QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
    for(unsigned int i = 0; i < selectedItems.size(); i++){
        bool add = true;
        for(unsigned int j = 0; j <  rootItems.size(); j++){
            RootTreeItem& rootItem = rootItems[j];
            if(selectedItems[i] == rootItem.item) {
                add = false;
            }
        }
        if(add)
            filteredItems.push_back(selectedItems[i]);
    }

    return filteredItems;
}

void SceneTree::deleteItem(ItemTMP* item){
    delete item->treeItem;

    itemsTMP.erase(remove(itemsTMP.begin(), itemsTMP.end(), *item),
                   itemsTMP.end());

    for(unsigned int i = 0;i < rootItems.size();i++){
        RootTreeItem& rootItem = rootItems[i];
        if(rootItem.isEmpty()){
            rootItem.destroy();
        }
    }
}

//-----------------------------//
//  PUBLIC
//-----------------------------//

bool SceneTree::objectExists(std::string name){
    for(unsigned int i = 0; i < itemsTMP.size(); i++){
        ItemTMP* item = &(itemsTMP[i]);
        if (name == item->object->getName())
            return true;
    }
    return false;
}

ItemTMP* SceneTree::getItemByName(std::string name){
    for(unsigned int i = 0; i < itemsTMP.size(); i++){
        ItemTMP* item = &(itemsTMP[i]);
        if (name == item->object->getName()){
            return item;
        }
    }
    return NULL;
}

ItemTMP* SceneTree::getItemByTree(QTreeWidgetItem* treeItem){
    for(unsigned int i = 0; i < itemsTMP.size(); i++){
        ItemTMP* item = &(itemsTMP[i]);
        if (treeItem == item->treeItem)
            return item;
    }
    return NULL;
}

void SceneTree::addObject(RenderBody* object, std::string type){
    QTreeWidgetItem* treeItem = new QTreeWidgetItem();
    treeItem->setText(0, QString::fromStdString(object->getName()));

    ItemTMP item(object, treeItem);
    item.type = type;
    this->itemsTMP.push_back(item);

    for(unsigned int i = 0;i < rootItems.size();i++){
        RootTreeItem& item = rootItems[i];
        if(type == item.type){
            item.init(this);
            item.item->addChild(treeItem);
        }
    }
}

SceneID SceneTree::deleteObject(string name){
    ItemTMP* item = getItemByName(name);
    if(item == NULL) {throw std::invalid_argument(name + " - No such Object");}
    SceneID id = item->object->getID();

    deleteItem(item);

    return id;
}

void SceneTree::changeName(string srcName, string dstName){
    ItemTMP* item = getItemByName(srcName);
    if(item == NULL) throw std::invalid_argument(srcName + " - No such Object");

    item->object->setName(dstName);
    item->treeItem->setText(0, QString::fromStdString(dstName));
}

void SceneTree::ShowContextMenu(const QPoint& pos){
    QList<QTreeWidgetItem *> selectedItems = filterSelectedItems();

    QPoint globalPos = this->mapToGlobal(pos);

    ContextMenu* menu = getMenuBasedOnItems(selectedItems);

    QAction* action = menu->show(globalPos);
    menu->handle(action, selectedItems);
}

//-----------------------------------------------------------//
//  EVENTS
//-----------------------------------------------------------//


void SceneTree::myitemActivated(QTreeWidgetItem* treeItem, int column){
    ItemTMP* item = getItemByTree(treeItem);
    if(item == NULL) return;

    //ObjectManager::getInstance().setActive(item->object->getID());
}

void SceneTree::myitemSelectionChanged(){
    QList<QTreeWidgetItem *> selectedItems = this->selectedItems();

    for(unsigned int i = 0;i < itemsTMP.size(); i++){
        ItemTMP item = itemsTMP[i];
        ObjectManager::getInstance().setDeactive(item.object->getID());
    }

    for(unsigned int i = 0; i < selectedItems.size();i++){
        ItemTMP* item = getItemByTree(selectedItems[i]);
        if(item == NULL) continue;
        ObjectManager::getInstance().setActive(item->object->getID());
    }
}

#include "moc_scene_tree.cpp"
