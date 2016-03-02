#ifndef OBJECTTREEWIDGET_H
#define OBJECTTREEWIDGET_H

#include <QTreeWidget>

class ObjectTreeWidget : public QTreeWidget
{
protected:
    void mousePressEvent(QMouseEvent *e);

public:
    ObjectTreeWidget(QWidget* parent);
};

#endif // OBJECTTREEWIDGET_H
