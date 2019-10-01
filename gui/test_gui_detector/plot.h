/* mainwindow.h
 * Copyright 2019 13 Aug
 * Kirill Pshenichnyi
 * This part of InMageS (Inelastic Magnet Scattering)
 * Software for Build analitical data and processing experemental data
 */

#ifndef PLOT_H
#define PLOT_H

#include <QMenu>
#include <QFileDialog>

#include "qcustomplot.h"

class Plot : public QCustomPlot
{
    Q_OBJECT
public:
    Plot(QWidget *parent=nullptr);
    ~Plot();

protected:
    bool x_log = false;
    bool y_log = false;

protected slots:
    void slot_sAxies_drag_zoom(QCPAxis *,QCPAxis::SelectablePart,QMouseEvent *);
    void slot_full_drag_zoom(QMouseEvent *);
    void slot_selectionChanged();
    void slot_contextMenuReq(QPoint p);

    void exportToPDF();
    void exportToBMP();
    void exportToJPG();
    void exportToPNG();

    void setXLog();
    void setYLog();

};

#endif // PLOT_H
