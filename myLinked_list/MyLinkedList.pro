TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    tests.cpp \
    linkedlist.cpp

HEADERS += \
    linkedlist.h

win32 {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
}
