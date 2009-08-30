TEMPLATE = lib
TARGET = common
CONFIG += qt console staticlib
DESTDIR = ../lib

MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = include/ui

DEPENDPATH += . icons include src ui
INCLUDEPATH += . include

# Input
HEADERS += include/SerialPort.h \
	include/SerialTesterDialog.h
	
FORMS += src/ui/SerialTesterDialog.ui

SOURCES += src/SerialPort.cpp \
	src/SerialTesterDialog.cpp
	
RESOURCES += icons/chrono.qrc
