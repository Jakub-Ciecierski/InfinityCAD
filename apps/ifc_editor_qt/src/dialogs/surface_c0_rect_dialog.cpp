#include "surface_c0_rect_dialog.h"
#include "ui_surface_c0_rect_dialog.h"

SurfaceC0RectDialog::SurfaceC0RectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfaceC0RectDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    initDefaultDataValue();
}

SurfaceC0RectDialog::~SurfaceC0RectDialog()
{
    delete ui;
}

//--------------------------//
//  PRIVATE
//--------------------------//

void SurfaceC0RectDialog::initDefaultDataValue(){
    QLineEdit* nEdit = ui->nLineEdit;
    QLineEdit* mEdit = ui->mLineEdit;
    QLineEdit* widthEdit = ui->widthLineEdit;
    QLineEdit* heightEdit = ui->heightLineEdit;

    QDoubleValidator* floatValidator =
            new QDoubleValidator(-1000.0, 1000.0, 2, NULL);
    QIntValidator* intValidator = new QIntValidator(100, 100, NULL);

    nEdit->setValidator(intValidator);
    mEdit->setValidator(intValidator);
    widthEdit->setValidator(floatValidator);
    heightEdit->setValidator(floatValidator);

    nEdit->setText("3");
    mEdit->setText("3");
    widthEdit->setText("0.5");
    heightEdit->setText("0.5");
}

//--------------------------//
//  PUBLIC
//--------------------------//


const SurfaceC0RectData& SurfaceC0RectDialog::getData(){
    return this->data;
}

//--------------------------//
//  PUBLIC SLOTS
//--------------------------//

void SurfaceC0RectDialog::cancelButtonClicked(){
    setResult(true);
    close();
}

void SurfaceC0RectDialog::okButtonClicked(){
    setResult(true);

    QLineEdit* nEdit = ui->nLineEdit;
    QLineEdit* mEdit = ui->mLineEdit;
    QLineEdit* widthEdit = ui->widthLineEdit;
    QLineEdit* heightEdit = ui->heightLineEdit;

    int n = nEdit->text().toInt();
    int m = mEdit->text().toInt();
    float width = widthEdit->text().toFloat();
    float height = heightEdit->text().toFloat();

    this->data = SurfaceC0RectData(n, m, width, height);

    close();
}
