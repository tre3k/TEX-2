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

    colorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
    colorMap->data()->setSize(nx, ny);
    colorMap->data()->setRange(QCPRange(-nx/2, nx/2), QCPRange(-ny/2, ny/2));

    colorScale = new QCPColorScale(plot);
    plot->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("count");

    colorMap->setGradient(QCPColorGradient::gpJet);

    marginGroup = new QCPMarginGroup(plot);
    plot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);


    colorMap->rescaleDataRange();
    plot->rescaleAxes();

    //QSpin Box and Label
    QHBoxLayout *toolLayout = new QHBoxLayout();
    centralLayout->addLayout(toolLayout);
    toolLayout->addWidget(new QLabel("time: "));

    spinBoxTime = new QDoubleSpinBox();
    toolLayout->addWidget(spinBoxTime);

    pushButtonMeasure = new QPushButton("start");
    toolLayout->addWidget(pushButtonMeasure);

    this->setCentralWidget(centralWidget);

    qRegisterMetaType("struct st_Data");

    // Thread

    th = new Thread();

    connect(pushButtonMeasure,SIGNAL(clicked()),
            this,SLOT(startMeasure()));
    connect(this,SIGNAL(send_options(options)),
            th,SLOT(accept_options(options)));
    connect(th,SIGNAL(add_value(st_Data)),
            this,SLOT(addValue(st_Data)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::startMeasure(){

    options opt;
    opt.time_in_sec = spinBoxTime->value();
    emit send_options(opt);
    th->start();

    return;
}

void MainWindow::addValue(st_Data dat){
    //if((x+nx/2) > nx || (x+nx/2) < 0) return;
    //if((y+ny/2) > ny || (y+ny/2) < 0) return;

    //matrix[x+nx/2][y+ny/2] = value;

    qDebug() << "asdasd" << dat.nx;

    /*
    for (int xIndex=0; xIndex<dat.nx; ++xIndex){
        for (int yIndex=0; yIndex<dat.ny; ++yIndex){
            colorMap->data()->setCell(xIndex, yIndex, dat.matrix[xIndex][yIndex]);
        }
    }

    colorMap->rescaleDataRange();
    plot->rescaleAxes();
    */
}
