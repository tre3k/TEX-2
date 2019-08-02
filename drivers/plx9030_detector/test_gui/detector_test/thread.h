#ifndef THREAD_H
#define THREAD_H

#include <QThread>

struct options{
    double time_in_sec;
};

class thread : public QThread
{
    Q_OBJECT
public:
    explicit thread(QObject *parent = nullptr);

signals:

public slots:
};

#endif // THREAD_H
