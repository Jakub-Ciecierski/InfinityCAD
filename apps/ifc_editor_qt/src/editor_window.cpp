#include "editor_window.h"
#include "ui_mainwindow.h"
#include <widgets/scene_list/context_menus/scene_cmenu_factory.h>
#include <dialogs/surface_settings_dialog.h>
#include <iostream>
#include <system/object_manager.h>

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>

using namespace std;

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupProperties();
    setupBinding();
    setupSplitters();

    objectsDialog = new ObjectsDialog();
}

EditorWindow::~EditorWindow()
{
    delete ui;

    delete objectsDialog;
}

//-------------------------//
//  PUBLIC METHODS
//-------------------------//

EditorWindow& EditorWindow::getInstance(){
    static EditorWindow instance;

    return instance;
}

Ui::MainWindow* EditorWindow::getUI(){
    return this->ui;
}

bool EditorWindow::showQuestionBox(string title, string text){
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, QString::fromStdString(title),
                                    QString::fromStdString(text),
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        return true;

      }
      return false;
}

void EditorWindow::showInfoBox(string title, string text){
    //QMessageBox::StandardButton reply;
    QMessageBox::question(this, QString::fromStdString(title),
                          QString::fromStdString(text),
                          QMessageBox::Yes|QMessageBox::No);
}

string EditorWindow::showInputBox(string title, string text,
                                  std::string initText){
    QInputDialog inputDialog;
    inputDialog.setOptions(QInputDialog::NoButtons);

    bool ok;
    QString reply =  inputDialog.getText(this ,QString::fromStdString(title),
                                        QString::fromStdString(text),
                                        QLineEdit::Normal,
                                        QString::fromStdString(initText), &ok);
    if (ok && !reply.isEmpty())
    {
        return reply.toStdString();
    }
    return "";
}

//-------------------------//
//  PUBLIC SLOTS
//-------------------------//

void EditorWindow::showObjectsDialog(){
    if(objectsDialog->isHidden()){
        objectsDialog->show();
        objectsDialog->move(this->x(),this->y());
    }else{
        objectsDialog->hide();
    }
}

void EditorWindow::netDensityAction(){
    SurfaceSettingsDialog surfaceSettings;
    surfaceSettings.exec();
}

void EditorWindow::saveSystem(){
    string filepath = showInputBox("Save", "Input File name");

    ObjectManager& objectManager = ObjectManager::getInstance();

    objectManager.saveSystem(filepath);
}

void EditorWindow::loadSystem(){
    ObjectManager& objectManager = ObjectManager::getInstance();

    //string filepath = showInputBox("Save", "Input File name");

    QString filepathQ =  QFileDialog::getOpenFileName();
    //std:string filepath = "jakub.mg";
    //std:string filepath = "curves.mg";
    std::cout << filepathQ.toStdString() << std::endl;
    objectManager.loadSystem(filepathQ.toStdString());
}

void EditorWindow::colapsButtomPressed(){
    ObjectManager& objectManager = ObjectManager::getInstance();

    objectManager.colapsSelectedPoints_NoRemove();
    //objectManager.colapsSelectedPoints();
}

void EditorWindow::mergeButtonPresses(){
    ObjectManager& objectManager = ObjectManager::getInstance();

    objectManager.mergeSelectedPoints();
}

//-------------------------//
//  PRIVATE METHODS
//-------------------------//

void EditorWindow::closeEvent(QCloseEvent *event){
    objectsDialog->hide();
}

void EditorWindow::setupProperties(){
    QLineEdit* x = ui->positionXInput;
    QLineEdit* y = ui->positionYInput;
    QLineEdit* z = ui->positionZInput;

    QDoubleValidator* validator =
            new QDoubleValidator(-1000.0, 1000.0, 2, NULL);

    x->setValidator(validator);
    y->setValidator(validator);
    z->setValidator(validator);

    x->setText("0.0");
    y->setText("0.0");
    z->setText("0.0");
}

void EditorWindow::setupBinding(){

}

void EditorWindow::setupSplitters(){
    ui->renderToPropertiesSplitter->setStretchFactor(10,0);
    ui->renderToPropertiesSplitter->setStretchFactor(2,1);

    ui->sceneProperitesSplitter->setStretchFactor(0,2);
    ui->sceneProperitesSplitter->setStretchFactor(1,1);
}
