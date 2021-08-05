QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DESTDIR = ../../WondronSoft/bin


message(current path: $$PWD)

INCLUDEPATH += $$PWD/../AlgoFramework
INCLUDEPATH += $$PWD/../FrameWork

include(../dependence/halcon17/halcon17.pri)

CONFIG(debug, debug|release) {
    message(AlgoFramework add frameworkd lib in debug mode)
    LIBS += $$PWD/../bin/framework.lib
    LIBS += $$PWD/../bin/AlgoFramework.lib
 } else {
    message(AlgoFramework add frameworkd lib in release mode)
    LIBS += $$PWD/../bin/framework.lib
    LIBS += $$PWD/../bin/AlgoFramework.lib
 }

SOURCES += \
    main.cpp \
    wondrondlg.cpp

HEADERS += \
    EchoInterface.h \
    wondrondlg.h

FORMS += \
    wondrondlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
