#-------------------------------------------------
#
# Project created by Gxx 2013-10-05T18:43:35
#
#-------------------------------------------------

QT       += core gui\
            sql\
            widgets\
            xml

TARGET = ../lib/SqlFunctions
TEMPLATE = lib
CONFIG += plugin

SOURCES += sqlfunctions.cpp \
    sqlfactory.cpp \
    xmlfunctions.cpp \
    QLinq.cpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

LIBS += -L../lib -lcommonfunction_c
INCLUDEPATH += ../hds
