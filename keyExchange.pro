QT       += core gui qml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


QMAKE_CC = gcc-4.9
QMAKE_CXX = g++-4.9

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS += -Wall

HEADERS += \
    NegativeIndexException.hpp \
    OutOfBoundsException.hpp \
    Matrix.hpp \
    IncompatibleMatricesException.hpp \
    MainWindow.hpp \
    parser.hpp \
    SessionB.hpp \
    SessionA.hpp \
    ComputationThread.hpp \
    main.hpp \
    grassmann.hpp

SOURCES += main.cpp \
    Matrix.cpp \
    parser.cpp \
    MainWindow.cpp \
    SessionB.cpp \
    SessionA.cpp \
    ComputationThread.cpp \
    grassmann.cpp


