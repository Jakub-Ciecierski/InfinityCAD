#include "surface_c0_dialog.h"
#include "ui_surface_c0_dialog.h"

SurfaceC0Dialog::SurfaceC0Dialog(const Type& type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfaceC0Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->type = type;

    initDefaultDataValue();
}

SurfaceC0Dialog::~SurfaceC0Dialog()
{
    delete ui;
}

//--------------------------//
//  PRIVATE
//--------------------------//

void SurfaceC0Dialog::initDefaultDataValue(){
    QLineEdit* nEdit = ui->nLineEdit;
    QLineEdit* mEdit = ui->mLineEdit;
    QLineEdit* widthEdit = ui->widthLineEdit;
    QLineEdit* heightEdit = ui->heightLineEdit;
    QLabel* widthLabel = ui->widthLabel;

    QPushButton* cancelButton = ui->cancelButton;
    QPushButton* okButton = ui->okButton;

    QDoubleValidator* floatValidator =
            new QDoubleValidator(-1000.0, 1000.0, 2, NULL);
    QIntValidator* intValidator = new QIntValidator(1, 1000, NULL);

    nEdit->setValidator(intValidator);
    mEdit->setValidator(intValidator);
    widthEdit->setValidator(floatValidator);
    heightEdit->setValidator(floatValidator);

    nEdit->setText("3");
    mEdit->setText("3");
    widthEdit->setText("0.5");
    heightEdit->setText("0.5");

    if(type == RB_SURFACE_C0_CYLIND_TYPE){
        widthLabel->setText("Radius");
        widthEdit->setText("0.1");
    }else{
        widthLabel->setText("Width");
    }

    okButton->setDefault(true);
    okButton->setAutoDefault(false);

    cancelButton->setDefault(false);
    cancelButton->setAutoDefault(false);

}

//--------------------------//
//  PUBLIC
//--------------------------//


const SurfaceC0Data& SurfaceC0Dialog::getData(){
    return this->data;
}

bool SurfaceC0Dialog::getResult(){
    return this->result;
}

//--------------------------//
//  PUBLIC SLOTS
//--------------------------//

void SurfaceC0Dialog::cancelButtonClicked(){
    result = false;
    close();
}

void SurfaceC0Dialog::okButtonClicked(){
    result = true;

    QLineEdit* nEdit = ui->nLineEdit;
    QLineEdit* mEdit = ui->mLineEdit;
    QLineEdit* widthEdit = ui->widthLineEdit;
    QLineEdit* heightEdit = ui->heightLineEdit;

    int n = nEdit->text().toInt();
    int m = mEdit->text().toInt();
    float width = widthEdit->text().toFloat();
    float height = heightEdit->text().toFloat();

    this->data = SurfaceC0Data(n, m, width, height);

    if(type == RB_SURFACE_C0_CYLIND_TYPE){
        data.radius = data.width;
    }

    close();
}
