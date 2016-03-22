#include "editor_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorWindow& w = EditorWindow::getInstance();
    w.show();

    return a.exec();
}
