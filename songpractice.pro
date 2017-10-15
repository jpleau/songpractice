QT       += widgets core gui multimedia
CONFIG += c++14

TARGET = songpractice

SOURCES += main.cpp mainwindow.cpp songlist.cpp song.cpp settings.cpp \
    player.cpp \
    songslider.cpp
HEADERS  += mainwindow.h songlist.h  song.h settings.h \
    player.h \
    songslider.h
FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += taglib sfml-audio

win32 {
    DEFINES += "TAGLIB_STATIC"
}

RESOURCES +=  resources.qrc

QMAKE_CXXFLAGS += -std=c++14