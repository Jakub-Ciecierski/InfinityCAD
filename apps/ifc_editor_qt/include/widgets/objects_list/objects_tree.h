#ifndef OBJECTSTREEWIDGET_H
#define OBJECTSTREEWIDGET_H

#include <QObject>
#include <QTreeWidget>


class ObjectsTree : public QTreeWidget
{
     Q_OBJECT

public:
    ObjectsTree(QWidget* parent);

private:
    void setupContextMenu();

public slots:
    void ShowContextMenu(const QPoint& pos);

};

#endif // OBJECTSTREEWIDGET_H
