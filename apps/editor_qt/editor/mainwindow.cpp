#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    allObjectsTreeManager = new AllObjectsTreeManager(ui->allObjectsTree);

    setupBinding();
    setupSplitters();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete allObjectsTreeManager;
}

void MainWindow::setupBinding(){

}

void MainWindow::setupSplitters(){
    ui->objectsTreeToRenderSplitter->setStretchFactor(0,1);
    ui->objectsTreeToRenderSplitter->setStretchFactor(1,20);

    ui->renderToPropertiesSplitter->setStretchFactor(0,10);
    ui->renderToPropertiesSplitter->setStretchFactor(1,2);

    ui->renderToPropertiesSplitter->setStyleSheet("QSplitter::handle{background: orange;}");
    ui->objectsTreeToRenderSplitter->setStyleSheet("QSplitter::handle{background: orange;}");

}
