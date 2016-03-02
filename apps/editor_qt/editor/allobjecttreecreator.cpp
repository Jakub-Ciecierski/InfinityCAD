#include "allobjectstreemanager.h"

#include <QMenu>

AllObjectsTreeManager::AllObjectsTreeManager(QTreeWidget* treeWidget)
{    
    this->treeWidget = treeWidget;

    setUp();
}

void AllObjectsTreeManager::setUp(){
    treeWidget->setColumnCount(1);
    setUpObjectsRoot();
    setUpRenderableObjectsRoot();
}

void AllObjectsTreeManager::setUpObjectsRoot(){
    // Create Root
    objectsRoot = new QTreeWidgetItem(this->treeWidget);
    objectsRoot->setText(0, "Objects");
    this->treeWidget->addTopLevelItem(objectsRoot);

    // Create Children.
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, "Camera (TODO)");
    objectsRoot->addChild(itm);
}

void AllObjectsTreeManager::setUpRenderableObjectsRoot(){
    // Create Root
    renderableObjectsRoot= new QTreeWidgetItem(this->treeWidget);
    renderableObjectsRoot->setText(0, "Renderable Objects");

    this->treeWidget->addTopLevelItem(renderableObjectsRoot);

    // Create Children
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, "Torus");
    renderableObjectsRoot->addChild(itm);

    //this->treeWidget->mousePressEvent();
}
