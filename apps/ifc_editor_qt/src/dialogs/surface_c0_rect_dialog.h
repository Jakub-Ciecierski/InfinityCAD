#ifndef SURFACE_C0_RECT_DIALOG_H
#define SURFACE_C0_RECT_DIALOG_H

#include <QDialog>

struct SurfaceC0RectData{
    int n;
    int m;

    float width;
    float height;

    SurfaceC0RectData(){
        this->n = 0;
        this->m = 0;
        this->width = 0;
        this->height = 0;
    }

    SurfaceC0RectData(int n, int m,
                      float width, float height){
        this->n = n;
        this->m = m;
        this->width = width;
        this->height = height;
    }
};

namespace Ui {
class SurfaceC0RectDialog;
}

class SurfaceC0RectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SurfaceC0RectDialog(QWidget *parent = 0);
    ~SurfaceC0RectDialog();

    const SurfaceC0RectData& getData();

public slots:

    void cancelButtonClicked();
    void okButtonClicked();

private:
    SurfaceC0RectData data;

    Ui::SurfaceC0RectDialog *ui;

    void initDefaultDataValue();
};

#endif // SURFACE_C0_RECT_DIALOG_H
