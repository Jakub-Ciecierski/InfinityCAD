#include "scene_tree.h"

#include <QMenu>
#include <editor_window.h>
#include <iostream>
#include "system/object_manager.h"
#include "system/ic_names.h"

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

    rootItems.push_back(RootTreeItem(RB_TORUS_NAME, "Toruses"));
    rootItems.push_back(RootTreeItem(RB_POINT_NAME, "Points"));
    rootItems.push_back(RootTreeItem(RB_BEZIER_NAME, "Bezier Curves"));
}

//-----------------------------//
//  PRIVATE
//-----------------------------//

void SceneTree::initTorusRoot(){
    if(torusTreeRoot != NULL) return;

    torusTreeRoot = new QTreeWidgetItem(this);
    torusTreeRoot->setText(0, "Toruses");
    this->addTopLevelItem(torusTreeRoot);
}

void SceneTree::initPointRoot(){
    if(pointTreeRoot != NULL) return;

    pointTreeRoot = new QTreeWidgetItem(this);
    pointTreeRoot->setText(0, "Points");
    this->addTopLevelItem(pointTreeRoot);
}

void SceneTree::setupContextMenu(){
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));
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
        if (name == item->object->getName())
            return item;
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


void SceneTree::addObject(RenderBody* object){
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(this);
    treeItem->setText(0, QString::fromStdString(object->getName()));

    this->addTopLevelItem(treeItem);

    ItemTMP item(object, treeItem);
    this->itemsTMP.push_back(item);
}

void SceneTree::addObject(RenderBody* object, std::string type){
    QTreeWidgetItem* treeItem = new QTreeWidgetItem();
    treeItem->setText(0, QString::fromStdString(object->getName()));

    ItemTMP item(object, treeItem);
    this->itemsTMP.push_back(item);

    for(unsigned int i = 0;i < rootItems.size();i++){
        RootTreeItem& item = rootItems[i];
        if(type == item.type){
            item.init(this);
            item.item->addChild(treeItem);
        }
    }
   /*

    if(type == RB_TORUS_NAME){
        initTorusRoot();
        torusTreeRoot->addChild(treeItem);

    }else if(type == RB_POINT_NAME){
        initPointRoot();
        pointTreeRoot->addChild(treeItem);
    }*/

}

SceneID SceneTree::deleteObject(string name){
    ItemTMP* item = getItemByName(name);
    if(item == NULL) throw std::invalid_argument(name + " - No such Object");
    deleteItem(item);

    return item->object->getID();
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
    ContextMenu& menu = EditorWindow::getInstance().
            getSceneListContextMenu();
    QAction* action = menu.show(globalPos);

    menu.handle(action, selectedItems);
}

//-----------------------------------------------------------//
//  EVENTS
//-----------------------------------------------------------//


void SceneTree::myitemActivated(QTreeWidgetItem* treeItem, int column){
    ItemTMP* item = getItemByTree(treeItem);
    if(item == NULL) return;

    ObjectManager::getInstance().setActive(item->object->getID());
}

void SceneTree::myitemSelectionChanged(){
    for(int i = 0;i < itemsTMP.size(); i++){
        ItemTMP item = itemsTMP[i];
        ObjectManager::getInstance().setDeactive(item.object->getID());
    }
}

#include "moc_scene_tree.cpp"
