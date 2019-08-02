#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>

#include "plx9030detector.h"

struct options{
    double time_in_sec;
};

struct st_Data{
    int nx,ny;
    unsigned int **matrix;
};

class Thread : public QThread
{
    Q_OBJECT
private:
    PLX9030Detector::plx9030Detector *pd;
    options opt;

public:
    explicit Thread(QObject *parent = nullptr);
    ~Thread();
    void run();

    st_Data dat;

signals:
    void add_value(st_Data);
public slots:
    void accept_options(options o);
};

#endif // THREAD_H
