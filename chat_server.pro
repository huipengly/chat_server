QT += core
QT -= gui
QT += network #加入网络支持

CONFIG += c++11

TARGET = chat_server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    my_tcp_server.cpp

HEADERS += \
    my_tcp_server.h
