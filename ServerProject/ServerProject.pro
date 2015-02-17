#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T22:59:19
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = ServerProject
CONFIG   += console
# CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mime_types.cpp \
    reply.cpp

LIBS += -L"/usr/local/boost_1_55_0/stage/lib/" -lboost_thread \
        -lrt


INCLUDEPATH += /usr/local/boost_1_55_0/

HEADERS += \
    header.hpp \
    mime_types.hpp \
    request.hpp \
    reply.hpp
