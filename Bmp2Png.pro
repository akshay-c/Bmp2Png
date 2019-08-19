QT       += core gui\
            concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bmp2Png
TEMPLATE = app

LIBS += -lQt5Concurrent

CONFIG += debug

SOURCES += src/Main.cpp\
        src/MainWindow.cpp \

HEADERS  += include/mainwindow.hpp\
        include/bmp2png.hpp \
