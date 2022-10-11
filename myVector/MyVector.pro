TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += myvector.cpp \
    tests.cpp

HEADERS += \
    myvector.h

win32 {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
}
