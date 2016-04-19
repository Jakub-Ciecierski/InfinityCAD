#include <widgets/objects_list/objects_tree_factory.h>
#include "system/ifc_types.h"

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
    renderableObjectsRoot->setExpanded(true);

    // Create Children
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(RB_TORUS_NAME));
    renderableObjectsRoot->addChild(itm);

    itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(RB_POINT_NAME));
    renderableObjectsRoot->addChild(itm);

    itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(RB_BEZIER_NAME));
    renderableObjectsRoot->addChild(itm);

    itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(RB_BSPLINE_NAME));
    renderableObjectsRoot->addChild(itm);

    itm = new QTreeWidgetItem();
    itm->setText(0, QString::fromStdString(RB_BSPLINE_INTERPOLATING_NAME));
    renderableObjectsRoot->addChild(itm);

}

void ObjectsTreeFactory::create(){
    setUp();
}
