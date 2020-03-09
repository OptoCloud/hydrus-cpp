QT += core gui opengl testlib sql multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

unix:!macx {
    # Include paths
    INCLUDEPATH += /usr/include/opencv4

    # Library paths
    LIBS += -L/usr/lib -L/usr/local -L/usr/local/lib -L/usr/local/lib/opencv4/3rdparty

    # OpenCV libraries
    LIBS += -lopencv_highgui -lopencv_img_hash -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

    # ZLib
    LIBS += -lz

    # OpenEXR library
    LIBS += -lIlmImf

    # Image libraries
    LIBS += -lpng16 -ljpeg -ltiff -llibjasper -llibwebp

    # Intel libraries
    LIBS += -ltbb -lippiw -lippicv

    # Dynamic linking library
    LIBS += -ldl

    # Optimization
    CONFIG(release, debug|release) {
      CONFIG -= -O1
      CONFIG -= -O2
      CONFIG *= -O3
    }
    CONFIG(debug, debug|release) {
      CONFIG -= -O1
      CONFIG -= -O2
      CONFIG -= -O3
    }
}
win32 {

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
    imageutils.cpp \
    main.cpp \
    mainwindow.cpp \
    sha256.cpp

HEADERS += \
    clientdb.h \
    hydrusanimation.h \
    hydrusimage.h \
    hydrusmedia.h \
    hydrusthumbnailitem.h \
    hydrusthumbnailview.h \
    hydrusvideo.h \
    imageutils.h \
    mainwindow.h \
    sha256.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
