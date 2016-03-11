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
    connect(ui->sceneRendererWidget, SIGNAL(ellipsoidARadiusChanged(int)),
            ui->ellipsoidARadiusSlider, SLOT(setValue(int)));
    connect(ui->sceneRendererWidget, SIGNAL(ellipsoidBRadiusChanged(int)),
            ui->ellipsoidBRadiusSlider, SLOT(setValue(int)));
    connect(ui->sceneRendererWidget, SIGNAL(ellipsoidCRadiusChanged(int)),
            ui->ellipsoidCRadiusSlider, SLOT(setValue(int)));
}
