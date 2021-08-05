TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets charts
TARGET          = $$qtLibraryTarget(aiplugin)
DESTDIR         = $$PWD/../plugins

INCLUDEPATH    += ../Wondron
INCLUDEPATH += $$PWD/../FrameWork

CONFIG(debug, debug|release) {
    message(AlgoFramework add frameworkd lib in debug mode)
    LIBS += $$PWD/../bin/framework.lib
 } else {
    message(AlgoFramework add frameworkd lib in release mode)
    LIBS += $$PWD/../bin/framework.lib
 }

include(../dependence/halcon17/halcon17.pri)

HEADERS += \
    WAiConstant.h \
    WAiConstant.h \
    aidatapro.h \
    aiplugin.h \
    aiwidget.h \
    cgetimagewidget.h

SOURCES += \
    aidatapro.cpp \
    aiplugin.cpp \
    aiwidget.cpp \
    cgetimagewidget.cpp

FORMS += \
    aiwidget.ui \
    cgetimagewidget.ui
