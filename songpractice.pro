QT       += widgets core gui multimedia
CONFIG += c++14

TARGET = songpractice

SOURCES += main.cpp mainwindow.cpp songlist.cpp song.cpp settings.cpp \
    songslider.cpp
HEADERS  += mainwindow.h songlist.h  song.h settings.h \
    songslider.h
FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += taglib

win32 {
    DEFINES += "TAGLIB_STATIC"
}

RESOURCES +=  resources.qrc

unix {
  isEmpty(PREFIX) {
    PREFIX = /usr
  }

  BINDIR = $$PREFIX/bin
  DATADIR = $$PREFIX/share
}

QMAKE_CXXFLAGS += -std=c++14
target.path = $$BINDIR/songpractice
desktop.path = $$DATADIR/applications
desktop.files = songpractice.desktop

INSTALLS += target desktop
