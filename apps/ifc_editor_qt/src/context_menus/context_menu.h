#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H


#include <QMenu>
#include <QPoint>
#include <context_menus/handler.h>
#include <QTreeWidgetItem>

class ContextMenu : public QMenu
{
protected:

    std::map<std::string, Handler> handlerMap;

public:
    ContextMenu();

    void addAction(std::string name);
    void addActionAndHandler(std::string name, Handler handler);
    void addHandler(std::string name, Handler handler);
    virtual QAction* show(const QPoint& pos);

    virtual void handle(const QAction* a,
                        const QList<QTreeWidgetItem *>& selected);

};

#endif // CONTEXTMENU_H
