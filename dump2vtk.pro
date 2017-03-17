TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QMAKE_CXXFLAGS += -g

SOURCES += main.cpp \
    lammpstextdumpreader.cpp \
    spatialbingrid.cpp \
    vtkwriter.cpp

HEADERS += \
    lammpstextdumpreader.h \
    spatialbingrid.h \
    vtkwriter.h
