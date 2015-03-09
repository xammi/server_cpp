#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T22:59:19
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = httpd
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mime_types.cpp \
    request_handler.cpp \
    request_parser.cpp \
    connection.cpp \
    server.cpp \
    response.cpp

HEADERS += \
    header.hpp \
    mime_types.hpp \
    request.hpp \
    request_handler.hpp \
    request_parser.hpp \
    connection.hpp \
    server.hpp \
    response.hpp

LIBS += -L"/usr/local/boost_1_55_0/stage/lib/" \
        -lboost_system \
        -lboost_thread \
        -lboost_program_options


INCLUDEPATH += /usr/local/boost_1_55_0/
