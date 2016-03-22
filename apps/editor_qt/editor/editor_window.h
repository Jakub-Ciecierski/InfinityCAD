#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <context_menus/context_menu.h>
#include <context_menus/context_menu_settings.h>

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
    ContextMenu& getSceneListContextMenu();

    Ui::MainWindow* getUI();

    bool showQuestionBox(std::string title, std::string text);
    void showInfoBox(std::string title, std::string text);
    std::string showInputBox(std::string title, std::string text);

private:
    explicit EditorWindow(QWidget *parent = 0);

    Ui::MainWindow *ui;

    ContextMenu* objectListContextMenu;
    ContextMenu* sceneListContextMenu;

    void setupProperties();

    void setupContextMenus();

    void setUpAllObjectsTree();
    void setupBinding();
    void setupSplitters();
};

#endif // MAINWINDOW_H
