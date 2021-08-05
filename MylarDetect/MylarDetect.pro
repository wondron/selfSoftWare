TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
DESTDIR         = $$PWD/../plugins
TARGET          = $$qtLibraryTarget(mylarplugin) #改变这个

INCLUDEPATH    += ../Wondron
INCLUDEPATH += $$PWD/../FrameWork
INCLUDEPATH += $$PWD/../FrameWork/utils
INCLUDEPATH += $$PWD/../AlgoFramework
INCLUDEPATH += $$PWD/../AlgoFramework/commonUsage
include(../dependence/halcon17/halcon17.pri)

message($$PWD)

CONFIG(debug, debug|release) {
    message(mylarDetectplugin add frameworkd lib in debug mode)
    LIBS += $$PWD/../bin/framework.lib
    LIBS += $$PWD/../bin/AlgoFramework.lib
 } else {
    message(mylarDetectplugin add frameworkd lib in release mode)
    LIBS += $$PWD/../bin/framework.lib
    LIBS += $$PWD/../bin/AlgoFramework.lib
 }



HEADERS += \
    cmylarmatch.h \
    cmylarmeasure.h \
    mylarplugin.h \
    test.h

SOURCES += \
    cmylarmatch.cpp \
    cmylarmeasure.cpp \
    mylarplugin.cpp \
    test.cpp

FORMS += \
    test.ui
