#ifndef SCENETREE_H
#define SCENETREE_H

#include <QObject>
#include <QTreeWidget>

#include <context_menus/context_menu.h>

#include <string>
#include <gm/scene/scene_id.h>
#include <gm/rendering/render_body.h>

struct ItemTMP{
    RenderBody* object;
    QTreeWidgetItem* treeItem;
    std::string type;

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

    const int TORUS_ROOT_INDEX = 0;
    const int POINT_ROOT_INDEX = 1;
    const int BEZIER_ROOT_INDEX = 2;
    std::vector<RootTreeItem> rootItems;

    QTreeWidgetItem* torusTreeRoot;
    QTreeWidgetItem* pointTreeRoot;

    void setupContextMenu();
    void setupRootItems();
    void setupBinding();

    ContextMenu* getMenuBasedOnItems(QList<QTreeWidgetItem *>& selectedItems);

    QList<QTreeWidgetItem *> filterSelectedItems();
    QList<QTreeWidgetItem *> filterSelectedItems(std::string type);

    void deleteItem(ItemTMP* item);

    virtual void dropEvent(QDropEvent * event);

public:
    SceneTree(QWidget* parent);

    bool objectExists(std::string name);

    ItemTMP* getItemByName(std::string name);
    ItemTMP* getItemByTree(QTreeWidgetItem* treeItem);

    void addObject(RenderBody* object, std::string type);
    SceneID deleteObject(std::string name);
    void changeName(std::string srcName, std::string dstName);

public slots:
    void ShowContextMenu(const QPoint& pos);
    void myitemActivated(QTreeWidgetItem* item, int column);
    void myitemSelectionChanged();
};

#endif // SCENETREE_H
