TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = gcc
QMAKE_CXXFLAGS += -std=c++0x -D_GLIBCXX_DEBUG
INCLUDEPATH += /home/antoshkka/boost_maintain/boost
SOURCES += test.cpp queue_with_min.hpp
LIBS += -lgtest -pthread

