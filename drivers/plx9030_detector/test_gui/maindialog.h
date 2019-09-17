#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "plot.h"

class maindialog : public QDialog
{
    Q_OBJECT
private:
    const int nx = 256;
    const int ny = 256;

public:
    explicit maindialog(QWidget *parent = nullptr);

    QVBoxLayout *mainLayout;
    QHBoxLayout *lowLayout;

    Plot *plot;
    QCPColorMap *color_map;
    QCPColorScale *color_scale;
    QPushButton *button_start;
    QDoubleSpinBox *spin_box;


signals:

public slots:
};

#endif // MAINDIALOG_H
