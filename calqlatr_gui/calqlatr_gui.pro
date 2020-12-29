TEMPLATE = app

QT += qml quick network
SOURCES += main.cpp \
    inputfile.cpp \
    outputfile.cpp \
    p_root.cpp \
    sys_ctl.cpp \
    dev_driver.cpp \
    configfile.cpp \
    qt_tcp_client.cpp \
    custom_data.cpp \
    cJSON.c \
    client.cpp

RESOURCES += calqlatr.qrc

OTHER_FILES =

#target.path = $$[QT_INSTALL_EXAMPLES]/demos/calqlatr
#target.path = ../
#INSTALLS += target
#TARGET = calqlatr

HEADERS += \
    inputfile.h \
    outputfile.h \
    p_root.h \
    sys_ctl.h \
    dev_driver.h \
    configfile.h \
    qt_tcp_client.h \
    custom_data.h \
    cJSON.h \
    client.h
