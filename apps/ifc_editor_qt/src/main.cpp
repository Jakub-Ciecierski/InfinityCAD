#include "editor_window.h"
#include <QApplication>
#include <system/object_manager.h>
#include <objects_dialog.h>
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorWindow& w = EditorWindow::getInstance();

    ObjectManager& objManager = ObjectManager::getInstance();
    //objManager.TEST_PERFORMANCE();
    //objManager.TEST_BSPLINE_INTERP();

    Ui::MainWindow* ui = w.getUI();
    ui->cudaCheckBox->toggle();
    ui->showPointsCheckbox->toggle();
    ui->cameraTypeCheckbox->toggle();

    w.show();

    return a.exec();
}
