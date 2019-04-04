include(../../gmock.pri)
include(../../gtest.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    test.cpp

LIBS += \
    Ws2_32.lib \
    Mswsock.lib \
    AdvApi32.lib

HEADERS +=
