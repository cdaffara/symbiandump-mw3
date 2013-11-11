symbian:load(qttest_p4)

TEMPLATE = app
TARGET = graphicsviewbenchmark

ROOTDIR = $$PWD/..

include($$ROOTDIR/widgets.pri)
include($$ROOTDIR/tests/itemrecyclinglist/itemrecyclinglist.pri)
include($$ROOTDIR/tests/simplelist/simplelist.pri)
include($$ROOTDIR/testautom/testautom.pri)

QT += script

DEPENDPATH += .
INCLUDEPATH += .

HEADERS += controller.h
SOURCES += main.cpp \
    controller.cpp

symbian: { 
    TARGET.EPOCHEAPSIZE = 10000 \
        48000000
}

DEPLOYMENT_PLUGIN += resourcemonplugin

# Causes crashing on Symbian if paging is enabled.
symbian {
    MMP_RULES -= PAGED
}