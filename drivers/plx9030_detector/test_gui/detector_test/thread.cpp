#include "thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{
    pd = new PLX9030Detector::plx9030Detector(QString("/dev/plxdetector0").toStdString());
    if(pd->status != PLX9030Detector::OK){
        qDebug() << "Erorr open file =(\n";
        delete pd;
    }

    dat.nx = 256;
    dat.ny = 256;
    dat.matrix = new unsigned int * [dat.nx];
    for(int i=0;i<dat.nx;i++) dat.matrix[i] = new unsigned int [dat.ny];


}

Thread::~Thread(){
    delete pd;
}

void Thread::run() {
    PLX9030Detector::four_value data4;
    int x = 0,y = 0;

    double cycle_delay = 1.0;
    int count = opt.time_in_sec/cycle_delay;


    for(int i=0;i<dat.nx;i++){
        for(int j=0;j<dat.ny;j++){
            dat.matrix[i][j] = 0;
        }
    }

    pd->init();

    for(int i=0;i<count;i++){
        pd->start();
        for(int j=0;j<1024;j++){
            data4 = pd->read4Value();
            x = data4.x2-data4.x1;
            y = data4.y2-data4.y1;
            x+=dat.nx/2;
            y+=dat.ny/2;

            //qDebug() << x << " " << y;

        }

        dat.matrix[3][3] = 12;

        this->msleep(cycle_delay*1000);
        pd->stop();

        emit add_value(dat);
    }




}


void Thread::accept_options(options o){
    opt = o;
}

