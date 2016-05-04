#include "objects_dialog.h"
#include "ui_objects_dialog.h"
#include <system/object_manager.h>

ObjectsDialog::ObjectsDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ObjectsDialog){
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
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

void ObjectsDialog::surfaceC0RectButtonClick(){
    addObject(RB_SURFACE_C0_RECT_TYPE);
}

void ObjectsDialog::surfaceC0CylindButtonClick(){
    addObject(RB_SURFACE_C0_CYLIND_TYPE);
}

//--------------------------//
//  PROTECTED
//--------------------------//

void ObjectsDialog::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        mMoving = true;
        mLastMousePosition = event->pos();
    }
}

void ObjectsDialog::mouseMoveEvent(QMouseEvent* event)
{
    if( event->buttons().testFlag(Qt::LeftButton) && mMoving)
    {
        this->move(event->globalX()-mLastMousePosition.x(),
                   event->globalY()-mLastMousePosition.y());
    }
}

void ObjectsDialog::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        mMoving = false;
    }
}


//--------------------------//
//  PRIVATE
//--------------------------//

void ObjectsDialog::addObject(const Type& type){
    ObjectManager& objectManager = ObjectManager::getInstance();
    objectManager.addObject(type);
}
