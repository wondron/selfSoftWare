TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
DESTDIR         = $$PWD/../plugins

TARGET          = $$qtLibraryTarget(cshplugin) #改变这个

INCLUDEPATH    += ../Wondron
INCLUDEPATH += $$PWD/../FrameWork
INCLUDEPATH += $$PWD/../FrameWork/utils
INCLUDEPATH += $$PWD/../AlgoFramework
INCLUDEPATH += $$PWD/../AlgoFramework/commonUsage

message($$PWD)

CONFIG(debug, debug|release) {
    message(cshDetectplugin add frameworkd lib in debug mode)
    LIBS += $$PWD/../bin/framework.lib
    LIBS += $$PWD/../bin/AlgoFramework.lib
 } else {
    message(cshDetectplugin add frameworkd lib in release mode)
    LIBS += $$PWD/../bin/framework.lib
    LIBS += $$PWD/../bin/AlgoFramework.lib
 }

include(../dependence/halcon17/halcon17.pri)
include(reverseSide/reverseSide.pri)
include(obverseSide/obverseSide.pri)
include(L68Side/L68Side.pri)

HEADERS += \
    cshplugin.h \
    imagedetectbase.h \
    imagegroupbase.h \
    test.h \
    totalalg.h \
    totaltestwidget.h \
    totaltestwidgetr.h

SOURCES += \
    cshplugin.cpp \
    imagedetectbase.cpp \
    imagegroupbase.cpp \
    test.cpp \
    totalalg.cpp \
    totaltestwidget.cpp \
    totaltestwidgetr.cpp

FORMS += \
    imagedetectbase.ui \
    test.ui \
    totalalg.ui \
    totaltestwidget.ui \
    totaltestwidgetr.ui
