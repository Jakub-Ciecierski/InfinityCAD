#ifndef OBJECTSTREEFACTORY_H
#define OBJECTSTREEFACTORY_H

#include <QtCore>
#include <QtGui>
#include <QTreeWidget>

class ObjectsTreeFactory
{
private:
    QTreeWidget* treeWidget;

    QTreeWidgetItem* objectsRoot;
    QTreeWidgetItem* renderableObjectsRoot;

    void setUp();
    void setUpObjectsRoot();
    void setUpRenderableObjectsRoot();

public:
    ObjectsTreeFactory(QTreeWidget* treeWidget);

    void create();
};

#endif // OBJECTSTREEFACTORY_H
