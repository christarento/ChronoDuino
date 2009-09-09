TEMPLATE = lib
TARGET = common
CONFIG += qt console staticlib
DESTDIR = ../lib

MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = include/ui

DEPENDPATH += . \
              include \
              include/ui
INCLUDEPATH += . \
               include \
               include/ui

# Input
HEADERS += include/SerialPort.h \
	include/SerialTesterDialog.h \
	include/SerialThread.h
	
FORMS += src/ui/SerialTesterDialog.ui

SOURCES += src/SerialPort.cpp \
	src/SerialTesterDialog.cpp \
	src/SerialThread.cpp
	
RESOURCES += icons/chrono.qrc
