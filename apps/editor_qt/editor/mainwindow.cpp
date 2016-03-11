#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    allObjectsTreeManager = new AllObjectsTreeManager(ui->allObjectsTree);

    setupBinding();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete allObjectsTreeManager;
}

void MainWindow::setupBinding(){

    ui->splitter->setStretchFactor(0, 20);
    ui->splitter->setStretchFactor(1, 0);
}
