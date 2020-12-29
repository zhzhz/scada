TEMPLATE = app

QT += qml quick
SOURCES += main.cpp \
    inputfile.cpp \
    outputfile.cpp \
    p_root.cpp

RESOURCES += calqlatr.qrc

OTHER_FILES =

#target.path = $$[QT_INSTALL_EXAMPLES]/demos/calqlatr
#target.path = ../
#INSTALLS += target
#TARGET = calqlatr

HEADERS += \
    inputfile.h \
    outputfile.h \
    p_root.h
