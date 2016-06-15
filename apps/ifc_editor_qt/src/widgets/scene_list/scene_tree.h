#ifndef SCENETREE_H
#define SCENETREE_H

#include <QObject>
#include <QTreeWidget>

#include <context_menus/context_menu.h>

#include <string>
#include "system/ifc_types.h"
#include <widgets/scene_list/entities/item.h>

class SceneTree : public QTreeWidget
{
    Q_OBJECT
private:

    std::vector<RootItem*> topRootItems;
    std::vector<Item*> allItems;

    bool showPoints;

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

    void moveItemUpWithinParent(Item* item);
    void moveItemDownWithinParent(Item* item);

    Item* getItemByName(std::string name);
    Item* getItemByTree(QTreeWidgetItem* treeItem);

    bool objectExists(std::string name);

    Item* addObject(RenderObject * object, const Type& type);
    SceneID deleteObject(Item* name);
    void changeName(Item* srcName, std::string dstName);


    void addChildItem(QTreeWidgetItem* bezierTreeItem,
                      QTreeWidgetItem* pointTreeItem);
    void addChildItem(Item* bezierName, Item* pointName);

    std::vector<Item*> getSelectedItems(const Type& type);
    std::vector<RenderObject*> getSelectedObjects();

    void activateObject(RenderObject * renderBody);
    void deactivateAll();

    const std::vector<Item*>& getAllItems();
public slots:
    void ShowContextMenu(const QPoint& pos);
    void myitemSelectionChanged();
    void showPointsToogled(bool value);

    void paramSliderMoved1(int v);
    void paramSliderMoved2(int v);
};

#endif // SCENETREE_H
