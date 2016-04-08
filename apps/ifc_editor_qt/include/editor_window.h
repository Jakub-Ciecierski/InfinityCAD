#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "context_menus/context_menu.h"
#include <widgets/objects_list/context_menus/objects_cmenu_settings.h>

namespace Ui {
class MainWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~EditorWindow();

    static EditorWindow& getInstance();

    ContextMenu& getObjectsListContextMenu();

    Ui::MainWindow* getUI();

    bool showQuestionBox(std::string title, std::string text);
    void showInfoBox(std::string title, std::string text);
    std::string showInputBox(std::string title, std::string text);

private:
    explicit EditorWindow(QWidget *parent = 0);

    Ui::MainWindow *ui;

    ContextMenu* objectListContextMenu;

    void setupProperties();

    void setupContextMenus();

    void setUpAllObjectsTree();
    void setupBinding();
    void setupSplitters();
};

#endif // MAINWINDOW_H
