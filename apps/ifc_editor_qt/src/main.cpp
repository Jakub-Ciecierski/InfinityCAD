#include "editor_window.h"
#include <QApplication>
#include <system/object_manager.h>
#include <objects_dialog.h>
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

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
