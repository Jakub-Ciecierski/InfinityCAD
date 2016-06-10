#ifndef SURFACE_C0_RECT_DIALOG_H
#define SURFACE_C0_RECT_DIALOG_H

#include <QDialog>
#include <system/ifc_types.h>
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

struct SurfaceC0Data{
    int n;
    int m;

    float radius;

    float width;
    float height;

    SurfaceAxis surfaceAxis;

    SurfaceC0Data(){
        this->n = 0;
        this->m = 0;

        this->radius = 0;

        this->width = 0;
        this->height = 0;
    }

    SurfaceC0Data(int n, int m,
                      float width, float height){
        this->n = n;
        this->m = m;

        this->radius = 0;

        this->width = width;
        this->height = height;
    }
};

namespace Ui {
class SurfaceC0Dialog;
}

class SurfaceC0Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit SurfaceC0Dialog(const Type& type = RB_SURFACE_C0_RECT_TYPE,
                             QWidget *parent = 0);
    ~SurfaceC0Dialog();

    const SurfaceC0Data& getData();

    bool getResult();

public slots:

    void cancelButtonClicked();
    void okButtonClicked();

private:
    bool result;

    Type type;

    SurfaceC0Data data;

    Ui::SurfaceC0Dialog *ui;

    void initDefaultDataValue();
};

#endif // SURFACE_C0_RECT_DIALOG_H
