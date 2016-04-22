#include "objects_dialog.h"
#include "ui_objects_dialog.h"
#include <system/object_manager.h>

ObjectsDialog::ObjectsDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ObjectsDialog){
    ui->setupUi(this);
}

ObjectsDialog::~ObjectsDialog(){
    delete ui;
}

//--------------------------//
//  PUBLIC SLOTS
//--------------------------//

void ObjectsDialog::pointButtonClick(){
    addObject(RB_POINT_TYPE);
}

void ObjectsDialog::torusButtonClick(){
    addObject(RB_TORUS_TYPE);
}

void ObjectsDialog::bezierC0ButtonClick(){
    addObject(RB_BEZIER_TYPE);
}

void ObjectsDialog::bsplineButtonClick(){
    addObject(RB_BSPLINE_TYPE);
}

void ObjectsDialog::bsplineInterpButtonClick(){
    addObject(RB_BSPLINE_INTERPOLATING_TYPE);
}

//--------------------------//
//  PRIVATE
//--------------------------//

void ObjectsDialog::addObject(const Type& type){
    ObjectManager& objectManager = ObjectManager::getInstance();
    objectManager.addObject(type);
}
