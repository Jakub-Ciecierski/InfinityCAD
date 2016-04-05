#ifndef SCENETREE_H
#define SCENETREE_H

#include <QObject>
#include <QTreeWidget>

#include <context_menus/context_menu.h>

#include <string>
#include <gm/scene/scene_id.h>
#include <gm/rendering/render_body.h>
#include "system/ic_names.h"
#include <widgets/scene_list/entities/item.h>

class SceneTree : public QTreeWidget
{
    Q_OBJECT
private:

    std::vector<RootItem*> topRootItems;
    std::vector<Item*> allItems;

    void setupContextMenu();
    void setupRootItems();
    void setupBinding();

    Item* getRootItem(const Type& type);
    std::vector<Item*> treeItemsToItems(QList<QTreeWidgetItem *>& treeItems);

    QList<QTreeWidgetItem *> filterOutTopRootSelectedItems();
    QList<QTreeWidgetItem *> filterSelectedItems(const Type& type);

    void deleteItem(Item* item);

    virtual void dropEvent(QDropEvent * event);

public:
    SceneTree(QWidget* parent);
    ~SceneTree();

    Item* getItemByName(std::string name);
    Item* getItemByTree(QTreeWidgetItem* treeItem);

    bool objectExists(std::string name);

    void addObject(RenderBody* object, const Type& type);
    SceneID deleteObject(Item* name);
    void changeName(Item* srcName, std::string dstName);


    void addPointToBezier(QTreeWidgetItem* bezierTreeItem,
                          QTreeWidgetItem* pointTreeItem);
    void addPointToBezier(Item* bezierName, Item* pointName);
    void moveItemWithinParent(Item* item);

    void activateObject(RenderBody* renderBody);
    void deactivateAll();

public slots:
    void ShowContextMenu(const QPoint& pos);
    void myitemActivated(QTreeWidgetItem* item, int column);
    void myitemSelectionChanged();
};

#endif // SCENETREE_H
