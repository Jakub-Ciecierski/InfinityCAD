#include <widgets/scene_list/entities/item.h>
#include <system/object_manager.h>
#include <iostream>

Item::Item(){
    this->object = NULL;

    clonedFrom = NULL;
    parent = NULL;
}

Item::Item(RenderObject * object, const Type& type){
    this->object = object;
    this->type = type;

    this->displayName = object->getName();

    treeItem = new QTreeWidgetItem();
    treeItem->setText(0, QString::fromStdString(displayName));

    clonedFrom = NULL;
    parent = NULL;
}

Item::~Item(){
    std::vector<Item*> tmpChildren = children;
    for(unsigned int i = 0;i < tmpChildren.size(); i++){
        Item* currChild = tmpChildren[i];
        children.erase(remove(children.begin(), children.end(), currChild),
                       children.end());

        if(currChild != NULL) {
             delete currChild;
        }
    }
    std::vector<Item*> tmpClones = clones;
    for(unsigned int i = 0;i < tmpClones.size(); i++){
        Item* currClone = tmpClones[i];
        if(currClone != NULL) {
            //delete currClone;
            ObjectManager& objManager = ObjectManager::getInstance();
            objManager.removeChildItem(currClone);
        }
    }
    if(isClone()){
        clonedFrom->removeClone(this);
    }
    if(parent != NULL){
        parent->children.erase(remove(parent->children.begin(),
                                      parent->children.end(), this),
                               parent->children.end());
    }
    if(treeItem != NULL){
        delete treeItem;
        treeItem = NULL;
    }
}

void Item::setName(std::string name){
    this->displayName = name;
    this->object->setName(name);
    treeItem->setText(0, QString::fromStdString(name));
    for(unsigned int i = 0;i < clones.size(); i++){
        Item* currClone = clones[i];
        currClone->setName(name);
    }
}

void Item::addChild(Item* child){
    child->parent = this;
    children.push_back(child);

    treeItem->addChild(child->treeItem);
    //treeItem->setExpanded(true);
    treeItem->setExpanded(false);
}

bool Item::removeChild(Item* child){
    bool ret = false;
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

Item* Item::makeClone(){
    // TODO proper clone type
    Item* item = new Item(this->object, this->type);
    item->clonedFrom = this;
    clones.push_back(item);

    return item;
}

void Item::removeClone(Item* clone){
    for(unsigned int i = 0; i < clones.size(); i++){
        if(clone == clones[i]){
            clones.erase(clones.begin() + i);
            return;
        }
    }
}

void Item::erase(std::vector<Item*>& allItems){
    allItems.erase(remove(allItems.begin(), allItems.end(), this),
                   allItems.end());
    for(unsigned int i = 0; i < clones.size(); i++){
        Item* clone = clones[i];
        allItems.erase(remove(allItems.begin(), allItems.end(), clone),
                     allItems.end());
    }
    for(unsigned int i = 0; i < children.size(); i++){
        Item* clone = children[i];
        allItems.erase(remove(allItems.begin(), allItems.end(), clone),
                       allItems.end());
    }
}

std::vector<Item*> Item::getOriginalChildrenItems(){
    std::vector<Item*> items;
    for(unsigned int i = 0; i < children.size(); i++){
        Item* child = children[i];
        items.push_back(child->clonedFrom);
    }
    return items;
}

std::vector<Item*> Item::getConnectedSurfaces(){
    std::vector<Item*> items;
    for(unsigned int i = 0; i < clones.size(); i++){
        Item* clone = clones[i];
        Item* parentClone = clone->parent;
        if(parentClone != NULL && isSurface(parentClone->type)){
            items.push_back(parentClone);
        }
    }
    return items;
}

ItemID Item::getID(QTreeWidget* treeWidget){
    ItemID id = createID(this, treeWidget);
    return id;
}

ItemID Item::createID(Item* item, QTreeWidget* treeWidget){
    std::vector<int> id_values;
    createID(item, treeWidget, id_values);

    ItemID id(id_values);

    return id;
}

void Item::createID(Item* item, QTreeWidget* treeWidget,
              std::vector<int>& id_values){
    Item* parent = item->parent;
    if(parent == NULL){
        QTreeWidgetItem* treeItem = item->treeItem;
        int index = treeWidget->indexOfTopLevelItem(treeItem);
        id_values.push_back(index);
        return;
    }else{
        int index = parent->treeItem->indexOfChild(item->treeItem);
        id_values.push_back(index);

        createID(parent, treeWidget, id_values);
    }
}

bool Item::isClone(){
    return (clonedFrom != NULL);
}

bool Item::operator==(Item& item) const{
    return (item.object == this->object);
}
