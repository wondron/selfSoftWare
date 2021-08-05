#Halcon17 lib path
HALCON_ROOT_PATH = $$PWD

INCLUDEPATH += \
    $$HALCON_ROOT_PATH/include \
    $$HALCON_ROOT_PATH/include/halconcpp

LIBS += -L$$HALCON_ROOT_PATH/lib -lhalconcpp
