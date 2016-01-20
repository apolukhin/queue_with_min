TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = gcc
QMAKE_CXXFLAGS += -std=c++0x -D_GLIBCXX_DEBUG
INCLUDEPATH += /home/antoshkka/boost_maintain/boost
SOURCES += test.cpp test_v2.cpp queue_with_min_v2.hpp queue_with_min_v1.hpp

LIBS += -lgtest -pthread

