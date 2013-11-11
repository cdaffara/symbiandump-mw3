QT += svg

contains(QT_CONFIG, opengl):QT += opengl
!symbian:CONFIG += debug

HEADERS += $$ROOTDIR/widgets/gvbwidget.h \
    $$ROOTDIR/widgets/abstractscrollarea.h \
    $$ROOTDIR/widgets/mainview.h \
    $$ROOTDIR/widgets/iconitem.h \
    $$ROOTDIR/widgets/label.h \
    $$ROOTDIR/widgets/listitem.h \
    $$ROOTDIR/widgets/scrollbar.h \
    $$ROOTDIR/widgets/simplelistview.h \
    $$ROOTDIR/widgets/scroller.h \
    $$ROOTDIR/widgets/scroller_p.h \
    $$ROOTDIR/widgets/button.h \
    $$ROOTDIR/widgets/menu.h \
    $$ROOTDIR/widgets/themeevent.h \
    $$ROOTDIR/widgets/theme.h \
    $$ROOTDIR/widgets/backgrounditem.h \
    $$ROOTDIR/widgets/topbar.h \
    $$ROOTDIR/widgets/commandline.h \
    $$ROOTDIR/widgets/dummydatagen.h \
    $$ROOTDIR/widgets/settings.h \

SOURCES += $$ROOTDIR/widgets/gvbwidget.cpp \
    $$ROOTDIR/widgets/abstractscrollarea.cpp \
    $$ROOTDIR/widgets/mainview.cpp \
    $$ROOTDIR/widgets/iconitem.cpp \
    $$ROOTDIR/widgets/label.cpp \
    $$ROOTDIR/widgets/listitem.cpp \
    $$ROOTDIR/widgets/scrollbar.cpp \
    $$ROOTDIR/widgets/simplelistview.cpp \
    $$ROOTDIR/widgets/scroller.cpp \
    $$ROOTDIR/widgets/button.cpp \
    $$ROOTDIR/widgets/menu.cpp \
    $$ROOTDIR/widgets/themeevent.cpp \
    $$ROOTDIR/widgets/theme.cpp \
    $$ROOTDIR/widgets/backgrounditem.cpp \
    $$ROOTDIR/widgets/topbar.cpp \
    $$ROOTDIR/widgets/commandline.cpp \
    $$ROOTDIR/widgets/dummydatagen.cpp \
    $$ROOTDIR/widgets/settings.cpp \

greaterThan(QT_MAJOR_VERSION, 3):greaterThan(QT_MINOR_VERSION, 5): {
    message(ListItemCache enabled in compilation)
    SOURCES += $$ROOTDIR/widgets/listitemcache.cpp
    HEADERS += $$ROOTDIR/widgets/listitemcache.h
}

RESOURCES += $$ROOTDIR/GraphicsViewBenchmark.qrc
INCLUDEPATH += $$ROOTDIR/widgets
