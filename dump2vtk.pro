TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    lammpstextdumpreader.cpp \
    spatialbingrid.cpp \
    vtkwriter.cpp

HEADERS += \
    lammpstextdumpreader.h \
    spatialbingrid.h \
    vtkwriter.h
