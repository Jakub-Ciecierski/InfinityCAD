#ifndef SCENETREE_H
#define SCENETREE_H

#include <QObject>
#include <QTreeWidget>

#include <string>
#include <gm/scene/scene_id.h>
#include <gm/rendering/render_body.h>

struct ItemTMP{
    RenderBody* object;
    QTreeWidgetItem* treeItem;

    ItemTMP(){}
    ItemTMP(RenderBody* obj,
            QTreeWidgetItem* treeItem){
        this->object = obj;
        this->treeItem = treeItem;
    }
    bool operator==(const ItemTMP& item) {
        return (item.object == this->object &&
                item.treeItem == this->treeItem);
    }
};

// ----------------------------------------------------------------

struct RootTreeItem{
    QTreeWidgetItem* item;
    std::string type;
    std::string displayName;

    RootTreeItem(){}

    RootTreeItem(const std::string type,
                 std::string displayName){
        this->item = NULL;
        this->type = type;
        this->displayName = displayName;
    }
    ~RootTreeItem(){
        if (item != NULL) delete item;
    }

    void init(QTreeWidget* treeWidget){
        if(item != NULL) return;

        item = new QTreeWidgetItem(treeWidget);
        item->setText(0, QString::fromStdString(displayName));
        treeWidget->addTopLevelItem(item);

        item->setExpanded(true);
    }

    bool isEmpty(){
        if(item == NULL || item->childCount() == 0)
            return true;
        return false;
    }

    void destroy(){
        delete item;
        item = NULL;
    }
};

// ----------------------------------------------------------------

class SceneTree : public QTreeWidget
{
    Q_OBJECT
private:
    std::vector<ItemTMP> itemsTMP;
    std::vector<RootTreeItem> rootItems;

    QTreeWidgetItem* torusTreeRoot;
    QTreeWidgetItem* pointTreeRoot;

    void initTorusRoot();
    void initPointRoot();

    void setupContextMenu();
    QList<QTreeWidgetItem *> filterSelectedItems();

    void deleteItem(ItemTMP* item);

public:
    SceneTree(QWidget* parent);

    bool objectExists(std::string name);

    ItemTMP* getItemByName(std::string name);
    ItemTMP* getItemByTree(QTreeWidgetItem* treeItem);

    void addObject(RenderBody* obj);
    void addObject(RenderBody* object, std::string type);

    SceneID deleteObject(std::string name);

    void changeName(std::string srcName, std::string dstName);

public slots:
    void ShowContextMenu(const QPoint& pos);
    void myitemActivated(QTreeWidgetItem* item, int column);
    void myitemSelectionChanged();
};

#endif // SCENETREE_H
