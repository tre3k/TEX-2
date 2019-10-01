#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QThread>
#include <QDebug>

#include "plot.h"
#include "plx9030detector.h"

#define nx 256
#define ny 256

class Thread : public QThread{
    Q_OBJECT
private:
    int t = 0;
public:
    explicit Thread(QWidget *parent = nullptr);
    void run();
public slots:
    void recvTime(int time){t = time;}
signals:
    void setCell(int i,int j, double value);
    void replot();
};


class MainDialog : public QDialog
{
    Q_OBJECT
private:

public:
    explicit MainDialog(QWidget *parent = nullptr);

    QVBoxLayout *mainLayout;
    QHBoxLayout *lowLayout;

    Plot *plot;
    QCPColorMap *color_map;
    QCPColorScale *color_scale;
    QPushButton *button_clean;
    QPushButton *button_start;
    QSpinBox *spin_box;
    Thread *th;

signals:
    void sendTime(int);

public slots:
    void pressButtonStart();

    void setCell(int i,int j,double value){
        if(nx/2+i < 0 || ny/2+j < 0) return;
        if(nx/2+i > nx || ny/2+j > nx) return;
        value += color_map->data()->cell(nx/2+i,ny/2+j);
        color_map->data()->setCell(nx/2+i,ny/2+j,value);
    }

    void replot(){
        color_map->rescaleDataRange();
        plot->rescaleAxes();
        color_map->rescaleAxes();
        plot->replot();
    }

    void cleanData(){
        color_map->data()->clear();
        color_map->data()->setSize(nx,ny);
        color_map->data()->setRange(QCPRange(-nx/2,nx/2),
                                    QCPRange(-ny/2,ny/2));
        replot();
    }
};


#endif // MAINDIALOG_H
