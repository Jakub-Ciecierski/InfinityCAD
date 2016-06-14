#include "intersectiondialog.h"
#include "ui_intersectiondialog.h"

IntersectionDialog::IntersectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntersectionDialog)
{
    ui->setupUi(this);
}

IntersectionDialog::~IntersectionDialog()
{
    delete ui;
}

Ui::IntersectionDialog* IntersectionDialog::getUI(){
    return ui;
}
