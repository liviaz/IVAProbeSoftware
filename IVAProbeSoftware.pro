#-------------------------------------------------
#
# Project created by QtCreator 2015-12-10T11:21:52
#
#-------------------------------------------------

QT       += core gui

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IVAProbeSoftware
TEMPLATE = app

INCLUDEPATH += "C:/Users/Admin/Desktop/IVA project/IVAProbeSoftware/DAQmxANSICDev/include"
INCLUDEPATH += "C:/Users/Admin/Desktop/IVA project/IVAProbeSoftware/DAQmxANSICDev/lib/msvc"
DEPENDPATH += "C:/Users/Admin/Desktop/IVA project/IVAProbeSoftware/DAQmxANSICDev/lib/msvc"

QMAKE_CXXFLAGS += -Wall

LIBS += -L"C:/Users/Admin/Desktop/IVA project/IVAProbeSoftware/DAQmxANSICDev/lib/msvc" \
    -l"NIDAQmx"

SOURCES += main.cpp \
        probegui.cpp \
    daqcontroller.cpp

HEADERS  += probegui.h \
    daqcontroller.h \
    DAQmxANSICDev/include/NIDAQmx.h

FORMS    += probegui.ui
