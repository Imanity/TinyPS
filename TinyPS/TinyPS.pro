#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T01:41:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TinyPS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += D:/Environment/opencv/build/include

CONFIG(debug, debug|release): {
LIBS += -L"D:/Environment/opencv/build/x64/vc14/lib" \
-lopencv_world330d
} else:CONFIG(release, debug|release): {
LIBS += -L"D:/Environment/opencv/build/x64/vc14/lib" \
-lopencv_world330
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mymat.cpp \
    dialogrotate.cpp \
    dialoghsl.cpp \
    dialoggraytransform.cpp \
    dialoggrayhistogram.cpp \
    dialogfilter.cpp \
    dialogbutterworth.cpp \
    slic.cpp \
    dialogslic.cpp \
    dialogfacedetection.cpp

HEADERS += \
        mainwindow.h \
    mymat.h \
    dialogrotate.h \
    dialoghsl.h \
    dialoggraytransform.h \
    dialoggrayhistogram.h \
    dialogfilter.h \
    dialogbutterworth.h \
    slic.h \
    dialogslic.h \
    dialogfacedetection.h

FORMS += \
        mainwindow.ui \
    dialogrotate.ui \
    dialoghsl.ui \
    dialoggraytransform.ui \
    dialoggrayhistogram.ui \
    dialogfilter.ui \
    dialogbutterworth.ui \
    dialogslic.ui \
    dialogfacedetection.ui
