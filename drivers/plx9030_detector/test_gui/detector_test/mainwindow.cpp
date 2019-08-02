#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    centralWidget = new QWidget();
    centralLayout = new QVBoxLayout();
    centralWidget->setLayout(centralLayout);

    plot=new QCustomPlot;
    plot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    plot->axisRect()->setupFullAxesBox(true);
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    centralLayout->addWidget(plot);


    this->setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::startMeasure(){
    return;
}
