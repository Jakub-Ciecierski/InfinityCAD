#ifndef ALLOBJECTTREECREATOR_H
#define ALLOBJECTTREECREATOR_H

#include <QtCore>
#include <QtGui>
#include <QTreeWidget>

class AllObjectsTreeManager
{
private:
    QTreeWidget* treeWidget;

    QTreeWidgetItem* objectsRoot;
    QTreeWidgetItem* renderableObjectsRoot;

    void setUp();
    void setUpObjectsRoot();
    void setUpRenderableObjectsRoot();
public:
    AllObjectsTreeManager(QTreeWidget* treeWidget);
};

#endif // ALLOBJECTTREECREATOR_H

