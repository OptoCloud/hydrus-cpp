QT       += core gui sql multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

unix:!macx {
    LIBS += -ldl -ljpeg
    QMAKE_CXXFLAGS += -lpthread -ldl

    Release {
        QMAKE_LFLAGS_RELEASE -= -O1
        QMAKE_LFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE -= -O1
        QMAKE_CXXFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE += -O3
    }
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientdb.cpp \
    hydrusanimation.cpp \
    hydrusimage.cpp \
    hydrusmedia.cpp \
    hydrusthumbnailitem.cpp \
    hydrusthumbnailview.cpp \
    hydrusvideo.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    clientdb.h \
    hydrusanimation.h \
    hydrusimage.h \
    hydrusmedia.h \
    hydrusthumbnailitem.h \
    hydrusthumbnailview.h \
    hydrusvideo.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Libraries
INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

# Linux
unix:!macx {
    LIBS += -L$$PWD/libs/ -lpHash -lpng
    PRE_TARGETDEPS += $$PWD/libs/linux_libpHash.a
    PRE_TARGETDEPS += $$PWD/libs/linux_libpng.a
}
