#include <QApplication>
#include "maindialog.h"

int main(int argc,char **argv){
    QApplication a(argc,argv);

    auto md = new MainDialog();
    md->show();

    return a.exec();
}
