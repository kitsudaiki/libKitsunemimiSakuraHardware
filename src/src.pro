QT -= qt core gui

TARGET = KitsunemimiSakuraHardware
TEMPLATE = lib
CONFIG += c++17
VERSION = 0.1.0

LIBS += -L../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../libKitsunemimiCommon/include

LIBS += -L../../libKitsunemimiCpu/src -lKitsunemimiCpu
LIBS += -L../../libKitsunemimiCpu/src/debug -lKitsunemimiCpu
LIBS += -L../../libKitsunemimiCpu/src/release -lKitsunemimiCpu
INCLUDEPATH += ../../libKitsunemimiCpu/include

INCLUDEPATH += $$PWD \
               $$PWD/../include

HEADERS += \
    ../include/libKitsunemimiSakuraHardware/cpu_core.h \
    ../include/libKitsunemimiSakuraHardware/cpu_socket.h \
    ../include/libKitsunemimiSakuraHardware/cpu_thread.h \
    ../include/libKitsunemimiSakuraHardware/host.h

SOURCES += \
    cpu_core.cpp \
    cpu_socket.cpp \
    cpu_thread.cpp \
    host.cpp

