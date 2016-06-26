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

    Ui::MainWindow* getUI();

    bool showQuestionBox(std::string title, std::string text);
    void showInfoBox(std::string title, std::string text);
    std::string showInputBox(std::string title, std::string text,
                             std::string initText = "sample");
protected:
    void closeEvent (QCloseEvent *event);

public slots:
    void showObjectsDialog();
    void netDensityAction();

    void saveSystem();
    void loadSystem();

    void colapsButtomPressed();
    void mergeButtonPresses();

private:
    explicit EditorWindow(QWidget *parent = 0);

    Ui::MainWindow *ui;
    ObjectsDialog* objectsDialog;

    void setupProperties();

    void setUpAllObjectsTree();
    void setupBinding();
    void setupSplitters();
};

#endif // MAINWINDOW_H
