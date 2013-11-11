TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH    += ../widgets
HEADERS         = resourcemonplugin.h
SOURCES         = resourcemonplugin.cpp
TARGET          = $$qtLibraryTarget(resourcemonplugin)
DESTDIR         = ../plugins

ROOTDIR = $$PWD/..

# install
target.path = $$ROOTDIR/plugins
sources.files = $$SOURCES $$HEADERS resourcemonplugin.pro
sources.path = $$ROOTDIR/resourcemonplugin
INSTALLS += target sources

symbian: {
    TARGET.EPOCALLOWDLLDATA=1
    TARGET.CAPABILITY = All -Tcb
    TARGET.UID3 = 0xE7b503bd
    MMP_RULES += "LIBRARY hal.lib"
}
