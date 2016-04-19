#include "editor_window.h"
#include <QApplication>
#include <system/object_manager.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorWindow& w = EditorWindow::getInstance();

    ObjectManager& objManager = ObjectManager::getInstance();
    //objManager.TEST_PERFORMANCE();
    objManager.TEST_BSPLINE_INTERP();

    w.show();

    return a.exec();
}
