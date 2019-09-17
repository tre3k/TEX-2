#include "maindialog.h"

maindialog::maindialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("test  Detector");

    mainLayout = new QVBoxLayout();
    lowLayout = new QHBoxLayout();

    plot = new Plot();
    color_map = new QCPColorMap(plot->xAxis,plot->yAxis);
    color_scale = new QCPColorScale(plot);
    spin_box = new QDoubleSpinBox();
    button_start = new QPushButton("start");

    mainLayout->addWidget(plot);
    lowLayout->addWidget(spin_box);
    lowLayout->addWidget(button_start);
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

}
