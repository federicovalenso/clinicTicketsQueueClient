#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T10:25:24
#
#-------------------------------------------------

QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Queue
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dlgsettings.cpp \
    appsettings.cpp

HEADERS += \
        mainwindow.h \
    dlgsettings.h \
    appsettings.h

FORMS += \
        mainwindow.ui \
    dlgsettings.ui

win32:RC_ICONS += icon.ico

RESOURCES += \
    res.qrc

DISTFILES += \
    qss/style.qss