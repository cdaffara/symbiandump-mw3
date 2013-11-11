symbian:load(qttest_p4)

TEMPLATE = app
TARGET = tst_GraphicsViewBenchmark

ROOTDIR = $$PWD/..

include($$ROOTDIR/widgets.pri)
include($$ROOTDIR/tests/itemrecyclinglist/itemrecyclinglist.pri)
include($$ROOTDIR/tests/simplelist/simplelist.pri)
include($$ROOTDIR/testautom/testautom.pri)

DEFINES += AUTO_TESTS
QT += script

DEPENDPATH += .
INCLUDEPATH += .

HEADERS += resultlogger.h \
    testthread.h \
    testcontroller.h \
    scriptrunner.h \
    benchmark.h \
    benchmarks.h \
    testfunctionresult.h

SOURCES += resultlogger.cpp \
    main.cpp \
    testthread.cpp \
    testcontroller.cpp \
    scriptrunner.cpp \
    benchmark.cpp \
    benchmarks.cpp \
    testfunctionresult.cpp

symbian::TARGET.EPOCHEAPSIZE = 10000 \
    48000000

DEPLOYMENT_PLUGIN += resourcemonplugin

OTHER_FILES = testscript.js \
    testautom.pri

# Causes crashing on Symbian if paging is enabled.
symbian {
    MMP_RULES -= PAGED
}