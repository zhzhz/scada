#-------------------------------------------------
#
# Project created by QtCreator 2020-07-07T10:05:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = com_net
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qt_tcp_client.cpp \
    client.cpp \
    configfile.cpp \
    cJSON.c \
    dev_driver.cpp \
    qt_tcp.cpp \
    custom_data.cpp

HEADERS += \
        mainwindow.h \
    qt_tcp_client.h \
    client.h \
    configfile.h \
    cJSON.h \
    dev_driver.h \
    qt_tcp.h \
    custom_data.h

#LIBS += C:\\Users\\zz\\Documents\\build-com_net-Desktop_Qt_5_9_8_MinGW_32bit-Debug\\debug\\Modbus.dll
#LIBS += D:\\qt_prj\\build-untitled-Desktop_Qt_5_12_10_MinGW_32_bit-Debug\\Modbus\\debug\\Modbus.dll
#LIBS += D:\\qt_prj\\build-untitled-Desktop_Qt_5_12_10_MinGW_32_bit-Debug\\Modbus\\debug\\Mitsubishi.dll

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
