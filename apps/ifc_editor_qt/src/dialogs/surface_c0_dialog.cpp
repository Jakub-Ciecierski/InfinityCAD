#include "surface_c0_dialog.h"
#include "ui_surface_c0_dialog.h"
#include <editor_window.h>

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

    nEdit->setText("1");
    mEdit->setText("1");
    widthEdit->setText("1.5");
    heightEdit->setText("1.5");

    if(type == RB_SURFACE_C0_CYLIND_TYPE ||
            type == RB_SURFACE_C2_CYLIND_TYPE){
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
    QCheckBox* isVerticalCheckBox = ui->isVerticalCheckBox;

    int n = nEdit->text().toInt();
    int m = mEdit->text().toInt();
    float width = widthEdit->text().toFloat();
    float height = heightEdit->text().toFloat();
    bool isVertical = isVerticalCheckBox->isChecked();

    this->data = SurfaceC0Data(n, m, width, height);
    if(isVertical)
        this->data.surfaceAxis = SurfaceAxis::VERTICAL;
    else
        this->data.surfaceAxis = SurfaceAxis::HORIZONTAL;

    if(type == RB_SURFACE_C0_CYLIND_TYPE ||
            type == RB_SURFACE_C2_CYLIND_TYPE){
        data.radius = data.width;
    }

    if(type == RB_SURFACE_C2_CYLIND_TYPE){
        if(data.m < 3){
            EditorWindow& window = EditorWindow::getInstance();
            std::string msg = "Column count \'m\' must be atleast 3. Setting m to 3";
            window.showInfoBox("Warning", msg);
            data.m = 3;
        }
    }
    close();
}
