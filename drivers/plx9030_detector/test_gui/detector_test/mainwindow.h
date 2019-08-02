#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>

#include "qcustomplot.h"
#include "thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QWidget *centralWidget;
    QVBoxLayout *centralLayout;
    QPushButton *pushButtonMeasure;
    QDoubleSpinBox *spinBoxTime;

    QCustomPlot *plot;
    QCPColorMap *colorMap;
    QCPColorScale *colorScale;
    QCPMarginGroup *marginGroup;

    Thread *th;

public slots:
    void startMeasure();
    void addValue(st_Data);

private:
   int nx=256,ny=256;

signals:
    void send_options(options);

};

#endif // MAINWINDOW_H
