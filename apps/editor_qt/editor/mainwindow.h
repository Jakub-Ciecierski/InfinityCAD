#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <allobjectstreemanager.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    AllObjectsTreeManager* allObjectsTreeManager;

    void setUpAllObjectsTree();
    void setupBinding();
};

#endif // MAINWINDOW_H
