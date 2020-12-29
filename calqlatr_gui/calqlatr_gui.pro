TEMPLATE = app

QT += qml quick
SOURCES += main.cpp \
    inputfile.cpp \
    outputfile.cpp

RESOURCES += calqlatr.qrc

OTHER_FILES =

#target.path = $$[QT_INSTALL_EXAMPLES]/demos/calqlatr
#target.path = ../
#INSTALLS += target
#TARGET = calqlatr

HEADERS += \
    inputfile.h \
    outputfile.h
