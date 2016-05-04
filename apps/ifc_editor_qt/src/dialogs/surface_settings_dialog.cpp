#include "surface_settings_dialog.h"
#include "ui_surface_settings_dialog.h"
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

SurfaceSettingsDialog::SurfaceSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfaceSettingsDialog)
{
    ui->setupUi(this);
}

SurfaceSettingsDialog::~SurfaceSettingsDialog()
{
    delete ui;
}

void SurfaceSettingsDialog::initDialog(){
    QLineEdit* uEdit = ui->uDivisionCountLineEdit;
    QLineEdit* vEdit = ui->vDivisionCountLineEdit;

    QIntValidator* intValidator = new QIntValidator(0, 1000, NULL);

    uEdit->setValidator(intValidator);
    vEdit->setValidator(intValidator);

    //Surface::del

}

void SurfaceSettingsDialog::cancelButtonClicked(){
    setResult(true);
    close();
}

void SurfaceSettingsDialog::okButtonClicked(){
    setResult(true);
    close();
}
