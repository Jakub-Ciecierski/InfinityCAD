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

SceneTree::~SceneTree(){
    for(unsigned int i = 0; i < topRootItems.size(); i++){
        delete topRootItems[i];
    }
}

//-----------------------------//
//  PRIVATE
//-----------------------------//

//-----------//
//  SETUP
//-----------//

void SceneTree::setupBinding(){
    this->connect(this, SIGNAL(itemPressed(QTreeWidgetItem*,int)),
                 this, SLOT(myitemActivated(QTreeWidgetItem*,int)));
    this->connect(this, SIGNAL(itemSelectionChanged()),
                 this, SLOT(myitemSelectionChanged()));
}

void SceneTree::setupRootItems(){
    topRootItems.push_back(new RootItem(RB_TORUS_TYPE, "Toruses"));
    topRootItems.push_back(new RootItem(RB_POINT_TYPE, "Points"));
    topRootItems.push_back(new RootItem(RB_BEZIER_TYPE, "Bezier Curves"));
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

RootItem* SceneTree::getRootItem(const Type& type){
    for(unsigned int i = 0;i < topRootItems.size(); i++){
        RootItem* rootItem = topRootItems[i];
        if(rootItem->type == type)
            return rootItem;
    }
    return NULL;
}

//-----------//
//  UTILITY
//-----------//

ContextMenu* SceneTree::getMenuBasedOnItems(
        QList<QTreeWidgetItem *>& selectedItems){
    SceneCMenuFactory& factory = SceneCMenuFactory::getInstance();

    int pointCount = 0;

    for(auto* treeItem : selectedItems){
        Item* item = getItemByTree(treeItem);
        if(item != NULL && item->type == RB_POINT_TYPE){
            pointCount++;
        }
    }
    // All selected are points
    if(pointCount == selectedItems.size()){
        RootItem* bezierRoot = getRootItem(RB_BEZIER_TYPE);
        if(bezierRoot == NULL)
            throw new std::invalid_argument("Bezier not implemented");
        return factory.getPointMenu(bezierRoot);
    }else{
        return factory.getDefaultMenu();
    }
}

QList<QTreeWidgetItem *> SceneTree::filterOutTopRootSelectedItems(){
    QList<QTreeWidgetItem *> filteredItems;

    QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
    for(int i = 0; i < selectedItems.size(); i++){
        bool add = true;
        for(unsigned int j = 0; j <  topRootItems.size(); j++){
            RootItem* rootItem = topRootItems[j];
            if(selectedItems[i] == rootItem->treeItem) {
                add = false;
            }
        }
        if(add)
            filteredItems.push_back(selectedItems[i]);
    }

    return filteredItems;
}

QList<QTreeWidgetItem *> SceneTree::filterSelectedItems(const Type& type){
    QList<QTreeWidgetItem *> filteredItems;

    QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
    for(int i = 0; i < selectedItems.size(); i++){
        Item* item  = getItemByTree(selectedItems[i]);
        if(item != NULL && item->type == type)
            filteredItems.push_back(selectedItems[i]);
    }

    return filteredItems;
}

void SceneTree::deleteItem(Item* item){
    item->erase(allItems);

    for(unsigned int i = 0;i < topRootItems.size();i++){
        RootItem* rootItem = topRootItems[i];
        rootItem->removeChild(item);

        if(rootItem->isEmpty()){
            rootItem->destroy();
        }
    }
}

void SceneTree::addPointToBezier(QTreeWidgetItem* bezierTreeItem,
                                 QTreeWidgetItem* pointTreeItem){
    Item* bezierItem = getItemByTree(bezierTreeItem);
    Item* pointItem = getItemByTree(pointTreeItem);
    if(bezierItem == NULL || pointItem == NULL ||
            bezierItem->type != RB_BEZIER_TYPE ||
            pointItem->type != RB_POINT_TYPE)
        throw new std::invalid_argument("Invalid Item types");

    // Check if point is already added
    for(unsigned int i = 0; i < bezierTreeItem->childCount();i++){
        QTreeWidgetItem* childTreeItem = bezierTreeItem->child(i);
        Item* childItem = getItemByTree(childTreeItem);
        if(childItem != NULL && *childItem == *pointItem)
            return;
    }
    Item* cloneItem = pointItem->makeClone();
    bezierItem->addChild(cloneItem);
    allItems.push_back(cloneItem);
}

//-----------//
//  EVENTS
//-----------//

void SceneTree::dropEvent(QDropEvent * event){
    QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        event->setDropAction(Qt::IgnoreAction);
        return;
    }
    QTreeWidgetItem* destinationTreeItem = itemFromIndex(index);
    Item* destItem = getItemByTree(destinationTreeItem);

    // Points to Bezier
    if(destItem != NULL && destItem->type == RB_BEZIER_TYPE) {
        QList<QTreeWidgetItem *> selectedPoints =
                filterSelectedItems(RB_POINT_TYPE);
        for(auto* selectedPoint : selectedPoints){
            addPointToBezier(destinationTreeItem, selectedPoint);
        }
    }
}

//-----------------------------//
//  PUBLIC
//-----------------------------//

bool SceneTree::objectExists(std::string name){
    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if (name == item->object->getName() && !item->isClone())
            return true;
    }
    return false;
}

Item* SceneTree::getItemByName(std::string name){
    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if (name == item->object->getName() && !item->isClone()){
            return item;
        }
    }
    return NULL;
}

Item* SceneTree::getItemByTree(QTreeWidgetItem* treeItem){
    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if (treeItem == item->treeItem)
            return item;
    }
    return NULL;
}

void SceneTree::addObject(RenderBody* object, const Type& type){
    Item* item = new Item(object, type);

    allItems.push_back(item);
    for(unsigned int i = 0;i < topRootItems.size();i++){
        RootItem* topRootItem = topRootItems[i];
        if(type == topRootItem->type){
            topRootItem->init(this);
            topRootItem->addChild(item);
        }
    }
}

SceneID SceneTree::deleteObject(string name){
    Item* item = getItemByName(name);
    if(item == NULL) {throw std::invalid_argument(name + " - No such Object");}
    SceneID id = item->object->getID();

    deleteItem(item);

    return id;
}

void SceneTree::changeName(string srcName, string dstName){
    Item* item = getItemByName(srcName);
    if(item == NULL) throw std::invalid_argument(srcName + " - No such Object");

    item->setName(dstName);
}

//-----------------------------//
//  SLOTS
//-----------------------------//

void SceneTree::ShowContextMenu(const QPoint& pos){
    QList<QTreeWidgetItem *> selectedItems = filterOutTopRootSelectedItems();

    QPoint globalPos = this->mapToGlobal(pos);

    ContextMenu* menu = getMenuBasedOnItems(selectedItems);

    QAction* action = menu->show(globalPos);
    menu->handle(action, selectedItems);
}


void SceneTree::myitemActivated(QTreeWidgetItem* treeItem, int column){
    Item* item = getItemByTree(treeItem);
    if(item == NULL) return;

    //ObjectManager::getInstance().setActive(item->object->getID());
}

void SceneTree::myitemSelectionChanged(){
    QList<QTreeWidgetItem *> selectedItems = this->selectedItems();

    for(unsigned int i = 0;i < allItems.size(); i++){
        Item* item = allItems[i];
        ObjectManager::getInstance().setDeactive(item->object->getID());
    }

    for(int i = 0; i < selectedItems.size();i++){
        Item* item = getItemByTree(selectedItems[i]);
        if(item == NULL) continue;
        ObjectManager::getInstance().setActive(item->object->getID());
    }
}

#include "moc_scene_tree.cpp"
