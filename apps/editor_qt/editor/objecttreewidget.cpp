#include "objecttreewidget.h"

#include <iostream>

ObjectTreeWidget::ObjectTreeWidget(QWidget* parent) :
    QTreeWidget(parent)
{

}

void ObjectTreeWidget::mousePressEvent(QMouseEvent *e){
    std::cout << "Mouse clicked" << std::endl;
//    e->accept();
}
