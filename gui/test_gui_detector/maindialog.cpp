#include "maindialog.h"

MainDialog::MainDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("test  Detector");

    mainLayout = new QVBoxLayout();
    lowLayout = new QHBoxLayout();

    plot = new Plot();
    color_map = new QCPColorMap(plot->xAxis,plot->yAxis);
    color_scale = new QCPColorScale(plot);
    spin_box = new QSpinBox();
    button_clean = new QPushButton("clean");
    button_start = new QPushButton("start");
    button_start->setMaximumWidth(70);
    button_clean->setMaximumWidth(70);

    mainLayout->addWidget(plot);
    lowLayout->addWidget(spin_box);
    lowLayout->addWidget(new QLabel(" sec"));
    lowLayout->addWidget(button_start);
    lowLayout->addWidget(button_clean);
    mainLayout->addLayout(lowLayout);

    this->setLayout(mainLayout);
    this->setMinimumSize(600,400);


    color_map->data()->setSize(nx,ny);
    color_map->data()->setRange(QCPRange(-nx/2,nx/2),
                                QCPRange(-ny/2,ny/2));
    plot->plotLayout()->addElement(0,1,color_scale);
    color_scale->setType(QCPAxis::atRight);
    color_map->setColorScale(color_scale);
    color_map->setGradient(QCPColorGradient::gpJet);

    plot->rescaleAxes(true);

    th = new Thread();

    connect(button_start,SIGNAL(clicked(bool)),this,SLOT(pressButtonStart()));
    connect(button_clean,SIGNAL(clicked(bool)),this,SLOT(cleanData()));
    connect(this,SIGNAL(sendTime(int)),th,SLOT(recvTime(int)));
    connect(th,SIGNAL(setCell(int,int,double)),this,SLOT(setCell(int,int,double)));
    connect(th,SIGNAL(replot()),this,SLOT(replot()));

}

void MainDialog::pressButtonStart(){
    emit sendTime(spin_box->value());
    th->start();
}

Thread::Thread(QWidget *parent) : QThread(parent){

}

void Thread::run(){
    qDebug() << "Run! time: " << t;
    int ix,iy;
    int sum_x,sum_y;
    double c_x,c_y;

    auto pd = new PLX9030Detector::plx9030Detector("/dev/plxdetector0");
    if(pd->status != PLX9030Detector::OK){
        qDebug() << "Error open /dev/plxdetector0";
        delete pd;
        return;
    }

    PLX9030Detector::four_value data4;

    for(int i=0;i<t;i++){
        pd->init();
        pd->start();
        QThread::sleep(1);
        pd->stop();

        do{
            data4 = pd->read4Value();
            if(!data4.correct) continue;
            sum_x = data4.x1+data4.x2;
            sum_y = data4.y1+data4.y2;

            c_x = (double)nx/sum_x;
            c_y = (double)ny/sum_y;

            ix = data4.x2-data4.x1;
            iy = data4.y2-data4.y1;

            qDebug() << c_x << ix*c_x << iy*c_y;

            ix = (int)(ix*c_x+0.5);
            iy = (int)(iy*c_y+0.5);

            qDebug() << ix << iy;

            emit setCell(ix,iy,1);
        }while(data4.correct);

        emit replot();
        qDebug() << "time: " << i+1;
    }

    qDebug() << "done";
    delete pd;
}
