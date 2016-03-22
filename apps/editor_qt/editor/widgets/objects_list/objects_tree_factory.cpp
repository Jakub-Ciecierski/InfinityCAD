#include "objects_tree_factory.h"

#include "widgets/objects_list/objects_settings.h"

ObjectsTreeFactory::ObjectsTreeFactory(QTreeWidget* treeWidget)
{
    this->treeWidget = treeWidget;
}

void ObjectsTreeFactory::setUp(){
    treeWidget->setColumnCount(1);
    setUpObjectsRoot();
    setUpRenderableObjectsRoot();
}

void ObjectsTreeFactory::setUpObjectsRoot(){
    // Create Root
    objectsRoot = new QTreeWidgetItem(this->treeWidget);
    objectsRoot->setText(0, QString::fromStdString(OBJ_ROOT_NAME));
    this->treeWidget->addTopLevelItem(objectsRoot);

    // Create Children.
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(OBJ_CAMERA_NAME));
    objectsRoot->addChild(itm);
}

void ObjectsTreeFactory::setUpRenderableObjectsRoot(){
    // Create Root
    renderableObjectsRoot= new QTreeWidgetItem(this->treeWidget);
    renderableObjectsRoot->setText(0, QString::fromStdString(RB_ROOT_NAME));

    this->treeWidget->addTopLevelItem(renderableObjectsRoot);

    // Create Children
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(RB_TORUS_NAME));
    renderableObjectsRoot->addChild(itm);

    itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(RB_POINT_NAME));
    renderableObjectsRoot->addChild(itm);

}

void ObjectsTreeFactory::create(){
    setUp();
}
