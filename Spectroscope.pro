QT       += core gui
QT       += network
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include(./systemCtrl/systemCtrl.pri)
include(./Controller/Controller.pri)

SOURCES += \
    Axis/XYAxis.cpp \
    Axis/XYView.cpp \
    Conn/Uart.cpp \
    Data/Data.cpp \
    Network/UDP.cpp \
    ThreadManager/threadmanager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Axis/XYAxis.h \
    Axis/XYView.h \
    Conn/Uart.h \
    Data/Data.h \
    Network/UDP.h \
    ThreadManager/threadmanager.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    systemCtrl/systemCtrl.pri
