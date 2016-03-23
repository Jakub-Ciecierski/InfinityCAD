#ifndef SCENETREE_H
#define SCENETREE_H

#include <QObject>
#include <QTreeWidget>

#include <string>
#include <gm/scene/scene_id.h>

struct Item{
    Item(){};
    Item(SceneID id,
         QTreeWidgetItem* treeItem,
         std::string name){
        this->id = id;
        this->treeItem = treeItem;
        this->name = name;
    };

    SceneID id;
    QTreeWidgetItem* treeItem;
    std::string name;
};

class SceneTree : public QTreeWidget
{
    Q_OBJECT
private:
    std::vector<Item> items;

    void setupContextMenu();

public:
    SceneTree(QWidget* parent);

    SceneID getID(std::string name);
    int getItemIndex(std::string name);
    int getItemIndex(QTreeWidgetItem* treeItem);

    void addObject(std::string name, SceneID id);
    SceneID deleteObject(std::string name);
    void changeName(std::string srcName, std::string dstName);

public slots:
    void ShowContextMenu(const QPoint& pos);

    void myitemActivated(QTreeWidgetItem* item, int column);

    void myitemSelectionChanged();
};

#endif // SCENETREE_H
