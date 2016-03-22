#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H


#include <QMenu>
#include <QPoint>
#include <context_menus/handler.h>
#include <QTreeWidgetItem>

class ContextMenu : public QMenu
{
public:
    ContextMenu();

    void addAction(std::string name, Handler handler);
    void addAction(std::string name);

    QAction* show(const QPoint& pos);

    void handle(const QAction* a, const QList<QTreeWidgetItem *>& selected);

private:

    std::map<std::string, Handler> handlerMap;
};

#endif // CONTEXTMENU_H
