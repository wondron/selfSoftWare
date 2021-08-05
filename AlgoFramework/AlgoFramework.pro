QT -= gui

TEMPLATE = lib
DEFINES += ALGOFRAMEWORK_LIBRARY

CONFIG += c++11
DESTDIR = ../../WondronSoft/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../dependence/halcon17/halcon17.pri)
include(commonUsage/commonUsage.pri)

SOURCES += \
    algoframework.cpp

HEADERS += \
    AlgoFramework_global.h \
    algoframework.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
