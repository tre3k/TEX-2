#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
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

    QCustomPlot *plot;
    QCPColorMap *colorMap;
    QCPColorScale *colorScale;
    QCPMarginGroup *marginGroup;

public slots:

    void startMeasure();

};

#endif // MAINWINDOW_H
