QT = websockets concurrent core network
QT -= gui
#QT += core
#QT += widgets
#QT += charts

CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
        OctaveServer.cpp \
        PlutoDevice.cpp \
        PlutoPWR.cpp \
        SensorServer.cpp
        # autotest_Pluto.cpp
        # main.cpp
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    OctaveServer.h \
    PlutoDevice.h \
    SensorServer.h

LIBS += -liio
