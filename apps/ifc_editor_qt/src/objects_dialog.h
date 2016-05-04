#ifndef OBJECT_WINDOW_H
#define OBJECT_WINDOW_H

#include <QDialog>
#include <system/ifc_types.h>

namespace Ui {
class ObjectsDialog;
}


class ObjectsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ObjectsDialog(QWidget *parent = 0);
    ~ObjectsDialog();

public slots:

    void pointButtonClick();
    void torusButtonClick();

    void bezierC0ButtonClick();
    void bsplineButtonClick();
    void bsplineInterpButtonClick();

    void surfaceC0RectButtonClick();

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    Ui::ObjectsDialog* ui;

    QPoint mLastMousePosition;
    bool mMoving;

    void addObject(const Type& type);
};



#endif // OBJECT_WINDOW_H
