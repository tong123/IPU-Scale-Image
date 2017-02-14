TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    filecustomdialog.cpp \
    ipu-scale.cpp

RESOURCES += qml.qrc \
    src.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
TARGET = main

HEADERS += \
    filecustomdialog.h \
    ipu-scale.h

DISTFILES +=
DEFINES += YOCT_SYS
contains( DEFINES, YOCT_SYS ) {
    INCLUDEPATH += "$$PWD/include/"
    LIBS += "$$PWD/libs/libipu.so" "$$PWD/libs/libipu.so.0"
}
