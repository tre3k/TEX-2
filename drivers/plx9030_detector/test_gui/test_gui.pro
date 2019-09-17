QT += core gui widgets printsupport

TARGET = test_gui
TEMPLATE = app

SOURCES += main.cpp \
    maindialog.cpp \
    qcustomplot.cpp \
    plot.cpp \
    plx9030detector.cpp

HEADERS += \
    maindialog.h \
    qcustomplot.h \
    plot.h \
    plx9030detector.h
