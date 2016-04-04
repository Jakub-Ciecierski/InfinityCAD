#ifndef SCENETREE_H
#define SCENETREE_H

#include <QObject>
#include <QTreeWidget>

#include <context_menus/context_menu.h>

#include <string>
#include <gm/scene/scene_id.h>
#include <gm/rendering/render_body.h>
#include "system/ic_names.h"

struct Item{
    RenderBody* object;
    QTreeWidgetItem* treeItem;
    Type type;

    std::vector<Item*> children;
    std::vector<Item*> clones;

    // NULL if it was not cloned;
    Item* clonedFrom;

    Item(const Type& type){
        this->object = NULL;
        this->type = type;

        treeItem = new QTreeWidgetItem();
        treeItem->setText(0, QString::fromStdString(object->getName()));

        clonedFrom = NULL;
    }

    Item(RenderBody* object, const Type& type){
        this->object = object;
        this->type = type;

        treeItem = new QTreeWidgetItem();
        treeItem->setText(0, QString::fromStdString(object->getName()));

        clonedFrom = NULL;
    }

    ~Item(){
        for(unsigned int i = 0;i < children.size(); i++){
            Item* currChild = children[i];
            if(!currChild->isClone()) delete currChild;
        }
        for(unsigned int i = 0;i < clones.size(); i++){
            Item* currClone = clones[i];
            delete currClone;
        }
        if(treeItem != NULL)
            delete treeItem;
    }

    void setName(std::string name){
        object->setName(name);
        treeItem->setText(0, QString::fromStdString(name));
        for(unsigned int i = 0;i < clones.size(); i++){
            Item* currClone = clones[i];
            currClone->setName(name);
        }
    }

    void addChild(Item* child){
        children.push_back(child);
        treeItem->addChild(child->treeItem);
        treeItem->setExpanded(true);
    }

    bool removeChild(Item* child){
        bool ret = false;
        // delete mem
        for(unsigned int i = 0;i < children.size(); i++){
            Item* currChild = children[i];
            if(currChild == child){
                children.erase(children.begin() + i);
                delete currChild;
                return true;
            }
        }

        // No direct children, search more
        for(unsigned int i = 0;i < children.size(); i++){
            Item* currChild = children[i];
            ret = currChild->removeChild(child);
            if(ret == true) return ret;
        }

        return ret;
    }

    Item* makeClone(){
        // TODO proper clone type
        Item* item = new Item(this->object, this->type);
        item->clonedFrom = this;
        clones.push_back(item);

        return item;
    }

    void erase(std::vector<Item*>& allItems){
        allItems.erase(remove(allItems.begin(), allItems.end(), this),
                       allItems.end());
        for(unsigned int i = 0; i < clones.size(); i++){
            Item* clone = clones[i];
            allItems.erase(remove(allItems.begin(), allItems.end(), clone),
                           allItems.end());
        }

    }

    bool isClone(){
        return (clonedFrom != NULL);
    }

    bool operator==(Item& item) const{
        return (item.object == this->object &&
                item.type == this->type);
    }
};


// ----------------------------------------------------------------

struct RootItem{
    QTreeWidgetItem* treeItem;
    Type type;
    std::string displayName;

    std::vector<Item*> children;

    RootItem(){}

    RootItem(const Type& type,
                 std::string displayName){
        this->treeItem = NULL;
        this->type = type;
        this->displayName = displayName;
    }
    ~RootItem(){
        for(unsigned int i = 0;i < children.size(); i++)
            delete children[i];

        if (treeItem != NULL) delete treeItem;
    }

    void init(QTreeWidget* treeWidget){
        if(treeItem != NULL) return;

        treeItem = new QTreeWidgetItem(treeWidget);
        treeItem->setText(0, QString::fromStdString(displayName));
        treeWidget->addTopLevelItem(treeItem);

        treeItem->setExpanded(true);
    }

    void addChild(Item* child){
        children.push_back(child);
        this->treeItem->addChild(child->treeItem);
    }

    bool removeChild(Item* child){
        bool ret = false;
        // delete mem
        for(unsigned int i = 0;i < children.size(); i++){
            Item* currChild = children[i];
            if(currChild == child){
                children.erase(children.begin() + i);
                delete currChild;
                return true;
            }
        }

        // No direct children, search more
        for(unsigned int i = 0;i < children.size(); i++){
            Item* currChild = children[i];
            ret = currChild->removeChild(child);
            if(ret == true) return ret;
        }

        return ret;
    }

    bool isEmpty(){
        if(treeItem == NULL || treeItem->childCount() == 0)
            return true;
        return false;
    }

    void destroy(){
        delete treeItem;
        treeItem = NULL;
    }
};


// ----------------------------------------------------------------

class SceneTree : public QTreeWidget
{
    Q_OBJECT
private:

    std::vector<RootItem*> topRootItems;
    std::vector<Item*> allItems;

    void setupContextMenu();
    void setupRootItems();
    void setupBinding();

    RootItem* getRootItem(const Type& type);

    ContextMenu* getMenuBasedOnItems(QList<QTreeWidgetItem *>& selectedItems);

    QList<QTreeWidgetItem *> filterOutTopRootSelectedItems();
    QList<QTreeWidgetItem *> filterSelectedItems(const Type& type);

    void deleteItem(Item* item);

    void addPointToBezier(QTreeWidgetItem* bezierTreeItem,
                          QTreeWidgetItem* pointTreeItem);

    virtual void dropEvent(QDropEvent * event);

public:
    SceneTree(QWidget* parent);
    ~SceneTree();

    Item* getItemByName(std::string name);
    Item* getItemByTree(QTreeWidgetItem* treeItem);

    bool objectExists(std::string name);

    void addObject(RenderBody* object, const Type& type);
    SceneID deleteObject(std::string name);
    void changeName(std::string srcName, std::string dstName);

public slots:
    void ShowContextMenu(const QPoint& pos);
    void myitemActivated(QTreeWidgetItem* item, int column);
    void myitemSelectionChanged();
};

#endif // SCENETREE_H
