QT       += widgets core gui multimedia
CONFIG += c++14

TARGET = songpractice

SOURCES += main.cpp mainwindow.cpp songlist.cpp song.cpp settings.cpp
HEADERS  += mainwindow.h songlist.h  song.h settings.h
FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += taglib

win32 {
    DEFINES += "TAGLIB_STATIC"
}

RESOURCES +=  resources.qrc