#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "context_menus/context_menu.h"
#include "objects_dialog.h"

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

public slots:
    void showObjectsDialog();

private:
    explicit EditorWindow(QWidget *parent = 0);

    Ui::MainWindow *ui;
    ObjectsDialog* objectsDialog;

    ContextMenu* objectListContextMenu;

    void setupProperties();

    void setupContextMenus();

    void setUpAllObjectsTree();
    void setupBinding();
    void setupSplitters();
};

#endif // MAINWINDOW_H
