#include "surface_settings_dialog.h"
#include "ui_surface_settings_dialog.h"
#include <infinity_cad/rendering/render_objects/surfaces/surface.h>

SurfaceSettingsDialog::SurfaceSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfaceSettingsDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    initDialog();
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

    std::string uStr;
    std::string vStr;

    uStr = std::to_string(Surface::uDivisionCount);
    vStr = std::to_string(Surface::vDivisionCount);

    QString uQStr = QString::fromStdString(uStr);
    QString vQStr = QString::fromStdString(vStr);

    uEdit->setText(uQStr);
    vEdit->setText(vQStr);

    QPushButton* cancelButton = ui->cancelButton;
    QPushButton* okButton = ui->okButton;

    okButton->setDefault(true);
    okButton->setAutoDefault(false);

    cancelButton->setDefault(false);
    cancelButton->setAutoDefault(false);
}

void SurfaceSettingsDialog::cancelButtonClicked(){
    setResult(false);
    close();
}

void SurfaceSettingsDialog::okButtonClicked(){
    QLineEdit* uEdit = ui->uDivisionCountLineEdit;
    QLineEdit* vEdit = ui->vDivisionCountLineEdit;

    int u = uEdit->text().toInt();
    int v = vEdit->text().toInt();

    Surface::uDivisionCount = u;
    Surface::vDivisionCount = v;

    setResult(true);
    close();
}
