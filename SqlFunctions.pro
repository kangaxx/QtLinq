#-------------------------------------------------
#
# Project created by Gxx 2013-10-05T18:43:35
#
#-------------------------------------------------

QT       += core gui\
            sql\
            widgets\
            xml

TARGET = SqlFunctions
TEMPLATE = lib
CONFIG += staticlib

SOURCES += sqlfunctions.cpp \
    sqlfactory.cpp \
    xmlfunctions.cpp \
    QLinq.cpp

HEADERS += sqlfunctions.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
